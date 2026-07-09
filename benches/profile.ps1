<#
.SYNOPSIS
    Windows Performance Profiling for pltxt2htm benchmarks.
.DESCRIPTION
    Provides three profiling modes:
      - wpr:    Uses Windows Performance Recorder for CPU sampling (WPA-compatible .etl)
      - simple: Uses Measure-Command for lightweight timing over N runs
      - etw:    Uses tracelog/xperf for ETW kernel-level CPU sampling (if available)
.PARAMETER Mode
    Profiling mode: wpr, simple, or etw (default: simple)
.PARAMETER Binary
    Path to benchmark executable to profile (default: ..\build\bench_end2end.exe)
.PARAMETER Iterations
    Number of iterations for simple mode (default: 10)
.PARAMETER Output
    Output directory for profiling artifacts (default: profile_results)
.EXAMPLE
    .\profile.ps1 -Mode simple -Binary .\build\bench_end2end.exe -Iterations 20
.EXAMPLE
    .\profile.ps1 -Mode wpr -Binary .\build\bench_parse.exe
#>

param(
    [ValidateSet('wpr', 'simple', 'etw')]
    [string]$Mode = 'simple',

    [string]$Binary = '',

    [int]$Iterations = 10,

    [string]$Output = 'profile_results'
)

$ErrorActionPreference = 'Stop'

# Resolve binary path
if (-not $Binary) {
    $PossiblePaths = @(
        '.\build\bench_end2end.exe',
        '..\build\bench_end2end.exe',
        'build\bench_end2end.exe'
    )
    foreach ($p in $PossiblePaths) {
        if (Test-Path $p) {
            $Binary = Resolve-Path $p
            break
        }
    }
    if (-not $Binary) {
        Write-Error "Could not find benchmark binary. Specify -Binary or build first."
        exit 1
    }
}

Write-Host "Profiling: $Binary" -ForegroundColor Cyan

# Ensure output directory
New-Item -ItemType Directory -Force -Path $Output | Out-Null

if ($Mode -eq 'simple') {
    Write-Host "Mode: simple (Measure-Command ×$Iterations)" -ForegroundColor Yellow
    $results = @()

    for ($i = 1; $i -le $Iterations; $i++) {
        Write-Progress -Activity "Running benchmark" -Status "Iteration $i of $Iterations" -PercentComplete (($i / $Iterations) * 100)
        $t = Measure-Command { & $Binary 2>$null }
        $results += [PSCustomObject]@{
            Run        = $i
            Total_ms   = $t.TotalMilliseconds
            Total_s    = $t.TotalSeconds
            CPU_ms     = $t.TotalMilliseconds  # Measure-Command doesn't separate CPU time
        }
    }

    $csvPath = Join-Path $Output "benchmark_results.csv"
    $results | Export-Csv -Path $csvPath -NoTypeInformation
    Write-Host "Results saved to $csvPath" -ForegroundColor Green

    # Summary stats
    $times = $results.Total_ms | Sort-Object
    $min = $times | Select-Object -First 1
    $max = $times | Select-Object -Last 1
    $median = $times[ [Math]::Floor($times.Count / 2) ]
    $avg = ($times | Measure-Object -Average).Average

    Write-Host "`n=== Summary ($Iterations runs) ===" -ForegroundColor Cyan
    Write-Host "  Min:    $($min.ToString('F3')) ms"
    Write-Host "  Max:    $($max.ToString('F3')) ms"
    Write-Host "  Median: $($median.ToString('F3')) ms"
    Write-Host "  Avg:    $($avg.ToString('F3')) ms"

    # Display as table
    $results | Format-Table -AutoSize

} elseif ($Mode -eq 'wpr') {
    Write-Host "Mode: WPR (Windows Performance Recorder)" -ForegroundColor Yellow

    $etlPath = Join-Path (Resolve-Path $Output) "profile.etl"

    # Check if WPR is available
    $wpr = Get-Command 'wpr.exe' -ErrorAction SilentlyContinue
    if (-not $wpr) {
        Write-Error "WPR not found. Install Windows Performance Toolkit (part of Windows ADK)."
        exit 1
    }

    Write-Host "Starting WPR CPU sampling..." -ForegroundColor Yellow
    & wpr -start CPU -filemode 2>&1 | Out-Null
    if ($LASTEXITCODE -ne 0) {
        Write-Error "WPR start failed. Try running as Administrator."
        exit 1
    }

    Write-Host "Running benchmark: $Binary" -ForegroundColor Yellow
    & $Binary

    Write-Host "Stopping WPR and saving to $etlPath..." -ForegroundColor Yellow
    & wpr -stop $etlPath 2>&1 | Out-Null

    Write-Host "`nProfile saved: $etlPath" -ForegroundColor Green
    Write-Host "Open with Windows Performance Analyzer (WPA) to analyze CPU hotspots."

} elseif ($Mode -eq 'etw') {
    Write-Host "Mode: ETW (tracelog/xperf)" -ForegroundColor Yellow

    $etlPath = Join-Path (Resolve-Path $Output) 'profile.etl'

    $xperf = Get-Command 'xperf.exe' -ErrorAction SilentlyContinue
    if (-not $xperf) {
        Write-Error "xperf not found. Install Windows Performance Toolkit (part of Windows ADK)."
        exit 1
    }

    Write-Host "Starting ETW kernel trace..." -ForegroundColor Yellow
    & xperf -on PROC_THREAD+LOADER+PROFILE -stackwalk Profile 2>&1 | Out-Null

    Write-Host "Running benchmark: $Binary" -ForegroundColor Yellow
    & $Binary

    Write-Host "Stopping trace and saving..." -ForegroundColor Yellow
    & xperf -d $etlPath 2>&1 | Out-Null

    Write-Host "`nProfile saved: $etlPath" -ForegroundColor Green
    Write-Host "Open with Windows Performance Analyzer (WPA) to analyze CPU hotspots."
}
