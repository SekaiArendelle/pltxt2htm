#!/usr/bin/env python3
"""Generate Windows API declarations and MSVC import-symbol aliases."""

from __future__ import annotations

import argparse
import dataclasses
import pathlib
import re
import shutil
import subprocess
from typing import Literal

ROOT = pathlib.Path(__file__).resolve().parents[1]
SYMBOL_ROOT = ROOT / "include" / "pltxt2htm" / "details" / "symbols"


@dataclasses.dataclass(frozen=True)
class Symbol:
    export_name: str
    return_type: str
    parameters: tuple[str, ...]
    nodiscard: bool = False
    calling_convention: Literal["stdcall", "cdecl"] = "stdcall"


@dataclasses.dataclass(frozen=True)
class Library:
    api: str
    name: str
    namespace: str
    definitions_header: str
    symbols: tuple[Symbol, ...]


LIBRARIES = (
    Library(
        api="nt",
        name="ntdll",
        namespace="pltxt2htm::details::symbols::nt",
        definitions_header="nt/definitions.hh",
        symbols=(
            Symbol(
                "RtlCaptureStackBackTrace",
                "Ushort",
                ("Ulong", "Ulong", "void**", "Ulong*"),
                nodiscard=True,
            ),
            Symbol(
                "RtlTryAcquireSRWLockExclusive",
                "Boolean",
                ("SrwLock*",),
                nodiscard=True,
            ),
            Symbol(
                "RtlAcquireSRWLockExclusive",
                "void",
                ("SrwLock*",),
            ),
            Symbol(
                "RtlReleaseSRWLockExclusive",
                "void",
                ("SrwLock*",),
            ),
            Symbol(
                "NtDuplicateObject",
                "NtStatus",
                ("Handle", "Handle", "Handle", "Handle*", "Ulong", "Ulong", "Ulong"),
                nodiscard=True,
            ),
            Symbol(
                "ZwDuplicateObject",
                "NtStatus",
                ("void *", "void *", "void *", "Handle *", "Ulong", "Ulong", "Ulong"),
                nodiscard=True,
            ),
            Symbol(
                "NtClose",
                "NtStatus",
                ("Handle",),
                nodiscard=True,
            ),
            Symbol(
                "ZwClose",
                "NtStatus",
                ("void *",),
                nodiscard=True,
            ),
            Symbol(
                "RtlNtStatusToDosError",
                "Ulong",
                ("Ulong",),
                nodiscard=True,
            ),
            Symbol(
                "NtCreateFile",
                "NtStatus",
                (
                    "Handle *",
                    "Ulong",
                    "ObjectAttributes const *",
                    "IoStatusBlock *",
                    "Long64 *",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "void *",
                    "Ulong",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwCreateFile",
                "NtStatus",
                (
                    "Handle *",
                    "Ulong",
                    "ObjectAttributes const *",
                    "IoStatusBlock *",
                    "Long64 *",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "void *",
                    "Ulong",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtCreateSection",
                "NtStatus",
                (
                    "Handle *",
                    "Ulong",
                    "ObjectAttributes const *",
                    "Long64 *",
                    "Ulong",
                    "Ulong",
                    "void *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwCreateSection",
                "NtStatus",
                (
                    "Handle *",
                    "Ulong",
                    "ObjectAttributes const *",
                    "Long64 *",
                    "Ulong",
                    "Ulong",
                    "void *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtQueryInformationProcess",
                "NtStatus",
                ("void *", "ProcessInformationClass", "void *", "Ulong", "Ulong *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwQueryInformationProcess",
                "NtStatus",
                ("void *", "ProcessInformationClass", "void *", "Ulong", "Ulong *"),
                nodiscard=True,
            ),
            Symbol(
                "NtWriteFile",
                "NtStatus",
                (
                    "void *",
                    "void *",
                    "IoApcRoutine",
                    "void *",
                    "IoStatusBlock *",
                    "void const *",
                    "Ulong",
                    "Long64 *",
                    "Ulong *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwWriteFile",
                "NtStatus",
                (
                    "void *",
                    "void *",
                    "IoApcRoutine",
                    "void *",
                    "IoStatusBlock *",
                    "void const *",
                    "Ulong",
                    "Long64 *",
                    "Ulong *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtReadFile",
                "NtStatus",
                (
                    "void *",
                    "void *",
                    "IoApcRoutine",
                    "void *",
                    "IoStatusBlock *",
                    "void *",
                    "Ulong",
                    "Long64 *",
                    "Ulong *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwReadFile",
                "NtStatus",
                (
                    "void *",
                    "void *",
                    "IoApcRoutine",
                    "void *",
                    "IoStatusBlock *",
                    "void *",
                    "Ulong",
                    "Long64 *",
                    "Ulong *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtQueryObject",
                "NtStatus",
                ("void *", "ObjectInformationClass", "void *", "Ulong", "Ulong *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwQueryObject",
                "NtStatus",
                ("void *", "ObjectInformationClass", "void *", "Ulong", "Ulong *"),
                nodiscard=True,
            ),
            Symbol(
                "NtQueryDirectoryFile",
                "NtStatus",
                (
                    "void *",
                    "void *",
                    "IoApcRoutine",
                    "void *",
                    "IoStatusBlock *",
                    "void *",
                    "Ulong",
                    "FileInformationClass",
                    "Boolean",
                    "UnicodeString *",
                    "Boolean",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwQueryDirectoryFile",
                "NtStatus",
                (
                    "void *",
                    "void *",
                    "IoApcRoutine",
                    "void *",
                    "IoStatusBlock *",
                    "void *",
                    "Ulong",
                    "FileInformationClass",
                    "Boolean",
                    "UnicodeString *",
                    "Boolean",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtQuerySection",
                "NtStatus",
                (
                    "void *",
                    "SectionInformationClass",
                    "void *",
                    "::std::size_t",
                    "::std::size_t *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwQuerySection",
                "NtStatus",
                (
                    "void *",
                    "SectionInformationClass",
                    "void *",
                    "::std::size_t",
                    "::std::size_t *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtQueryInformationFile",
                "NtStatus",
                (
                    "void *",
                    "IoStatusBlock *",
                    "void *",
                    "Ulong",
                    "FileInformationClass",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwQueryInformationFile",
                "NtStatus",
                (
                    "void *",
                    "IoStatusBlock *",
                    "void *",
                    "Ulong",
                    "FileInformationClass",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtSetInformationFile",
                "NtStatus",
                (
                    "void *",
                    "IoStatusBlock *",
                    "void *",
                    "Ulong",
                    "FileInformationClass",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwSetInformationFile",
                "NtStatus",
                (
                    "void *",
                    "IoStatusBlock *",
                    "void *",
                    "Ulong",
                    "FileInformationClass",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtWaitForSingleObject",
                "NtStatus",
                ("void *", "Boolean", "Long64 *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwWaitForSingleObject",
                "NtStatus",
                ("void *", "Boolean", "Long64 *"),
                nodiscard=True,
            ),
            Symbol(
                "NtWaitForMultipleObjects",
                "NtStatus",
                ("Ulong", "void *const *", "WaitType", "Boolean", "Long64 *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwWaitForMultipleObjects",
                "NtStatus",
                ("Ulong", "void *const *", "WaitType", "Boolean", "Long64 *"),
                nodiscard=True,
            ),
            Symbol(
                "NtSetSystemTime",
                "NtStatus",
                ("Long64 *", "Long64 *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwSetSystemTime",
                "NtStatus",
                ("Long64 *", "Long64 *"),
                nodiscard=True,
            ),
            Symbol(
                "NtCreateProcess",
                "NtStatus",
                (
                    "Handle *",
                    "Ulong",
                    "ObjectAttributes const *",
                    "void *",
                    "Boolean",
                    "void *",
                    "void *",
                    "void *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwCreateProcess",
                "NtStatus",
                (
                    "Handle *",
                    "Ulong",
                    "ObjectAttributes const *",
                    "void *",
                    "Boolean",
                    "void *",
                    "void *",
                    "void *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "RtlDosPathNameToNtPathName_U",
                "Boolean",
                (
                    "char16_t const *",
                    "UnicodeString *",
                    "char16_t **",
                    "RtlRelativeNameU *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "RtlDosPathNameToNtPathName_U_WithStatus",
                "NtStatus",
                (
                    "char16_t const *",
                    "UnicodeString *",
                    "char16_t **",
                    "RtlRelativeNameU *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "RtlNtPathNameToDosPathName",
                "NtStatus",
                ("Ulong", "RtlUnicodeStringBuffer *", "Ulong *", "char16_t **"),
                nodiscard=True,
            ),
            Symbol(
                "RtlFreeUnicodeString",
                "void",
                ("UnicodeString *",),
            ),
            Symbol(
                "RtlInitializeCriticalSection",
                "NtStatus",
                ("void *",),
                nodiscard=True,
            ),
            Symbol(
                "RtlEnterCriticalSection",
                "NtStatus",
                ("void *",),
                nodiscard=True,
            ),
            Symbol(
                "RtlTryEnterCriticalSection",
                "Ulong",
                ("void *",),
                nodiscard=True,
            ),
            Symbol(
                "RtlLeaveCriticalSection",
                "NtStatus",
                ("void *",),
                nodiscard=True,
            ),
            Symbol(
                "RtlDeleteCriticalSection",
                "NtStatus",
                ("void *",),
                nodiscard=True,
            ),
            Symbol(
                "RtlCreateUserThread",
                "NtStatus",
                (
                    "void *",
                    "void *",
                    "Boolean",
                    "Ulong",
                    "::std::size_t",
                    "::std::size_t",
                    "UserThreadStartRoutine",
                    "void *",
                    "Handle *",
                    "ClientId *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtResumeThread",
                "NtStatus",
                ("void *", "Ulong *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwResumeThread",
                "NtStatus",
                ("void *", "Ulong *"),
                nodiscard=True,
            ),
            Symbol(
                "NtLockFile",
                "NtStatus",
                (
                    "void *",
                    "void *",
                    "IoApcRoutine",
                    "void *",
                    "IoStatusBlock *",
                    "Long64 *",
                    "Long64 *",
                    "Ulong",
                    "Boolean",
                    "Boolean",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwLockFile",
                "NtStatus",
                (
                    "void *",
                    "void *",
                    "IoApcRoutine",
                    "void *",
                    "IoStatusBlock *",
                    "Long64 *",
                    "Long64 *",
                    "Ulong",
                    "Boolean",
                    "Boolean",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtUnlockFile",
                "NtStatus",
                ("void *", "IoStatusBlock *", "Long64 *", "Long64 *", "Ulong"),
                nodiscard=True,
            ),
            Symbol(
                "ZwUnlockFile",
                "NtStatus",
                ("void *", "IoStatusBlock *", "Long64 *", "Long64 *", "Ulong"),
                nodiscard=True,
            ),
            Symbol(
                "NtFlushBuffersFile",
                "NtStatus",
                ("void *", "IoStatusBlock *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwFlushBuffersFile",
                "NtStatus",
                ("void *", "IoStatusBlock *"),
                nodiscard=True,
            ),
            Symbol(
                "NtFlushBuffersFileEx",
                "NtStatus",
                ("void *", "Ulong", "void *", "Ulong", "IoStatusBlock *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwFlushBuffersFileEx",
                "NtStatus",
                ("void *", "Ulong", "void *", "Ulong", "IoStatusBlock *"),
                nodiscard=True,
            ),
            Symbol(
                "DbgPrint",
                "Ulong",
                ("char const *", "..."),
                nodiscard=True,
                calling_convention="cdecl",
            ),
            Symbol(
                "DbgPrintEx",
                "Ulong",
                ("Ulong", "Ulong", "char const *", "..."),
                nodiscard=True,
                calling_convention="cdecl",
            ),
            Symbol(
                "RtlCreateProcessParameters",
                "NtStatus",
                (
                    "RtlUserProcessParameters **",
                    "UnicodeString const *",
                    "UnicodeString const *",
                    "UnicodeString const *",
                    "UnicodeString const *",
                    "void *",
                    "UnicodeString const *",
                    "UnicodeString const *",
                    "UnicodeString const *",
                    "UnicodeString const *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "RtlCreateProcessParametersEx",
                "NtStatus",
                (
                    "RtlUserProcessParameters **",
                    "UnicodeString const *",
                    "UnicodeString const *",
                    "UnicodeString const *",
                    "UnicodeString const *",
                    "void *",
                    "UnicodeString const *",
                    "UnicodeString const *",
                    "UnicodeString const *",
                    "UnicodeString const *",
                    "Ulong",
                ),
                nodiscard=True,
            ),
            Symbol(
                "RtlDestroyProcessParameters",
                "NtStatus",
                ("RtlUserProcessParameters *",),
                nodiscard=True,
            ),
            Symbol(
                "NtCreateUserProcess",
                "NtStatus",
                (
                    "Handle *",
                    "Handle *",
                    "Ulong",
                    "Ulong",
                    "ObjectAttributes const *",
                    "ObjectAttributes const *",
                    "Ulong",
                    "Ulong",
                    "RtlUserProcessParameters *",
                    "PsCreateInfo *",
                    "PsAttributeList *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwCreateUserProcess",
                "NtStatus",
                (
                    "Handle *",
                    "Handle *",
                    "Ulong",
                    "Ulong",
                    "ObjectAttributes const *",
                    "ObjectAttributes const *",
                    "Ulong",
                    "Ulong",
                    "RtlUserProcessParameters *",
                    "PsCreateInfo *",
                    "PsAttributeList *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtMapViewOfSection",
                "NtStatus",
                (
                    "void *",
                    "void *",
                    "void **",
                    "::std::size_t",
                    "::std::size_t",
                    "Long64 *",
                    "::std::size_t *",
                    "SectionInherit",
                    "Ulong",
                    "Ulong",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwMapViewOfSection",
                "NtStatus",
                (
                    "void *",
                    "void *",
                    "void **",
                    "::std::size_t",
                    "::std::size_t",
                    "Long64 *",
                    "::std::size_t *",
                    "SectionInherit",
                    "Ulong",
                    "Ulong",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtUnmapViewOfSection",
                "NtStatus",
                ("void *", "void *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwUnmapViewOfSection",
                "NtStatus",
                ("void *", "void *"),
                nodiscard=True,
            ),
            Symbol(
                "NtReadVirtualMemory",
                "NtStatus",
                ("void *", "void *", "void *", "::std::size_t", "::std::size_t *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwReadVirtualMemory",
                "NtStatus",
                ("void *", "void *", "void *", "::std::size_t", "::std::size_t *"),
                nodiscard=True,
            ),
            Symbol(
                "NtWriteVirtualMemory",
                "NtStatus",
                ("void *", "void *", "void *", "::std::size_t", "::std::size_t *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwWriteVirtualMemory",
                "NtStatus",
                ("void *", "void *", "void *", "::std::size_t", "::std::size_t *"),
                nodiscard=True,
            ),
            Symbol(
                "RtlAcquirePebLock",
                "NtStatus",
                (),
                nodiscard=True,
            ),
            Symbol(
                "RtlReleasePebLock",
                "NtStatus",
                (),
                nodiscard=True,
            ),
            Symbol(
                "NtAllocateVirtualMemory",
                "NtStatus",
                (
                    "void *",
                    "void **",
                    "::std::size_t",
                    "::std::size_t *",
                    "Ulong",
                    "Ulong",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAllocateVirtualMemory",
                "NtStatus",
                (
                    "void *",
                    "void **",
                    "::std::size_t",
                    "::std::size_t *",
                    "Ulong",
                    "Ulong",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtFreeVirtualMemory",
                "NtStatus",
                ("void *", "void **", "::std::size_t *", "Ulong"),
                nodiscard=True,
            ),
            Symbol(
                "ZwFreeVirtualMemory",
                "NtStatus",
                ("void *", "void **", "::std::size_t *", "Ulong"),
                nodiscard=True,
            ),
            Symbol(
                "RtlInitUnicodeString",
                "void",
                ("UnicodeString *", "char16_t const *"),
            ),
            Symbol(
                "CsrClientCallServer",
                "NtStatus",
                ("void *", "void *", "Ulong", "Ulong"),
                nodiscard=True,
            ),
            Symbol(
                "NtQuerySystemInformation",
                "NtStatus",
                ("SystemInformationClass", "void *", "Ulong", "Ulong *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwQuerySystemInformation",
                "NtStatus",
                ("SystemInformationClass", "void *", "Ulong", "Ulong *"),
                nodiscard=True,
            ),
            Symbol(
                "NtQueryVolumeInformationFile",
                "NtStatus",
                ("void *", "IoStatusBlock *", "void *", "Ulong", "FsInformationClass"),
                nodiscard=True,
            ),
            Symbol(
                "ZwQueryVolumeInformationFile",
                "NtStatus",
                ("void *", "IoStatusBlock *", "void *", "Ulong", "FsInformationClass"),
                nodiscard=True,
            ),
            Symbol(
                "NtFsControlFile",
                "NtStatus",
                (
                    "void *",
                    "void *",
                    "IoApcRoutine",
                    "void *",
                    "IoStatusBlock *",
                    "Ulong",
                    "void *",
                    "Ulong",
                    "void *",
                    "Ulong",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwFsControlFile",
                "NtStatus",
                (
                    "void *",
                    "void *",
                    "IoApcRoutine",
                    "void *",
                    "IoStatusBlock *",
                    "Ulong",
                    "void *",
                    "Ulong",
                    "void *",
                    "Ulong",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtCreateNamedPipeFile",
                "NtStatus",
                (
                    "Handle *",
                    "Ulong",
                    "ObjectAttributes const *",
                    "IoStatusBlock *",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "Long64 *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwCreateNamedPipeFile",
                "NtStatus",
                (
                    "Handle *",
                    "Ulong",
                    "ObjectAttributes const *",
                    "IoStatusBlock *",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "Ulong",
                    "Long64 *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtQueryPerformanceCounter",
                "NtStatus",
                ("Long64 *", "Long64 *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwQueryPerformanceCounter",
                "NtStatus",
                ("Long64 *", "Long64 *"),
                nodiscard=True,
            ),
            Symbol(
                "RtlGetSystemTimePrecise",
                "Ulong64",
                (),
                nodiscard=True,
            ),
            Symbol(
                "NtQueryInformationThread",
                "NtStatus",
                ("void *", "ThreadInformationClass", "void *", "Ulong", "Ulong *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwQueryInformationThread",
                "NtStatus",
                ("void *", "ThreadInformationClass", "void *", "Ulong", "Ulong *"),
                nodiscard=True,
            ),
            Symbol(
                "RtlAdjustPrivilege",
                "NtStatus",
                ("Ulong", "Boolean", "Boolean", "Boolean *"),
                nodiscard=True,
            ),
            Symbol(
                "NtCreateEvent",
                "NtStatus",
                (
                    "Handle *",
                    "Ulong",
                    "ObjectAttributes const *",
                    "EventType",
                    "Boolean",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwCreateEvent",
                "NtStatus",
                (
                    "Handle *",
                    "Ulong",
                    "ObjectAttributes const *",
                    "EventType",
                    "Boolean",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtSetEvent",
                "NtStatus",
                ("void *", "Long *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwSetEvent",
                "NtStatus",
                ("void *", "Long *"),
                nodiscard=True,
            ),
            Symbol(
                "NtResetEvent",
                "NtStatus",
                ("void *", "Long *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwResetEvent",
                "NtStatus",
                ("void *", "Long *"),
                nodiscard=True,
            ),
            Symbol(
                "NtSetInformationObject",
                "NtStatus",
                ("void *", "ObjectInformationClass", "void *", "Ulong"),
                nodiscard=True,
            ),
            Symbol(
                "ZwSetInformationObject",
                "NtStatus",
                ("void *", "ObjectInformationClass", "void *", "Ulong"),
                nodiscard=True,
            ),
            Symbol(
                "NtQueryAuxiliaryCounterFrequency",
                "NtStatus",
                ("Ulong64 *",),
                nodiscard=True,
            ),
            Symbol(
                "ZwQueryAuxiliaryCounterFrequency",
                "NtStatus",
                ("Ulong64 *",),
                nodiscard=True,
            ),
            Symbol(
                "RtlDetermineDosPathNameType_U",
                "RtlPathType",
                ("char16_t const *",),
                nodiscard=True,
            ),
            Symbol(
                "RtlGetFullPathName_U",
                "Ulong",
                ("char16_t const *", "Ulong", "char16_t *", "char16_t **"),
                nodiscard=True,
            ),
            Symbol(
                "AlpcInitializeMessageAttribute",
                "NtStatus",
                (
                    "Ulong",
                    "AlpcMessageAttributes *",
                    "::std::size_t",
                    "::std::size_t *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "AlpcGetMessageAttribute",
                "void*",
                ("AlpcMessageAttributes*", "Ulong"),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcCreatePort",
                "NtStatus",
                ("Handle *", "ObjectAttributes *", "AlpcPortAttributes *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcCreatePort",
                "NtStatus",
                ("Handle *", "ObjectAttributes *", "AlpcPortAttributes *"),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcConnectPort",
                "NtStatus",
                (
                    "Handle *",
                    "UnicodeString const *",
                    "ObjectAttributes *",
                    "AlpcPortAttributes *",
                    "Ulong",
                    "void *",
                    "PortMessage *",
                    "::std::size_t *",
                    "AlpcMessageAttributes *",
                    "AlpcMessageAttributes *",
                    "Long64 *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcConnectPort",
                "NtStatus",
                (
                    "Handle *",
                    "UnicodeString const *",
                    "ObjectAttributes *",
                    "AlpcPortAttributes *",
                    "Ulong",
                    "void *",
                    "PortMessage *",
                    "::std::size_t *",
                    "AlpcMessageAttributes *",
                    "AlpcMessageAttributes *",
                    "Long64 *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcSendWaitReceivePort",
                "NtStatus",
                (
                    "void *",
                    "Ulong",
                    "PortMessage *",
                    "AlpcMessageAttributes *",
                    "PortMessage *",
                    "::std::size_t *",
                    "AlpcMessageAttributes *",
                    "Long64 *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcSendWaitReceivePort",
                "NtStatus",
                (
                    "void *",
                    "Ulong",
                    "PortMessage *",
                    "AlpcMessageAttributes *",
                    "PortMessage *",
                    "::std::size_t *",
                    "AlpcMessageAttributes *",
                    "Long64 *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcAcceptConnectPort",
                "NtStatus",
                (
                    "Handle *",
                    "void *",
                    "Ulong",
                    "ObjectAttributes *",
                    "AlpcPortAttributes *",
                    "void *",
                    "PortMessage *",
                    "AlpcMessageAttributes *",
                    "Boolean",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcAcceptConnectPort",
                "NtStatus",
                (
                    "Handle *",
                    "void *",
                    "Ulong",
                    "ObjectAttributes *",
                    "AlpcPortAttributes *",
                    "void *",
                    "PortMessage *",
                    "AlpcMessageAttributes *",
                    "Boolean",
                ),
                nodiscard=True,
            ),
            Symbol(
                "AlpcGetHeaderSize",
                "Ulong",
                ("Ulong",),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcImpersonateClientOfPort",
                "NtStatus",
                ("void *", "PortMessage *", "void *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcImpersonateClientOfPort",
                "NtStatus",
                ("void *", "PortMessage *", "void *"),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcImpersonateClientContainerOfPort",
                "NtStatus",
                ("void *", "PortMessage *", "Ulong"),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcImpersonateClientContainerOfPort",
                "NtStatus",
                ("void *", "PortMessage *", "Ulong"),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcDisconnectPort",
                "NtStatus",
                ("void *", "Ulong"),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcDisconnectPort",
                "NtStatus",
                ("void *", "Ulong"),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcCreatePortSection",
                "NtStatus",
                (
                    "void *",
                    "Ulong",
                    "void *",
                    "::std::size_t",
                    "Handle *",
                    "::std::size_t *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcCreatePortSection",
                "NtStatus",
                (
                    "void *",
                    "Ulong",
                    "void *",
                    "::std::size_t",
                    "Handle *",
                    "::std::size_t *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcCreateSectionView",
                "NtStatus",
                ("void *", "Ulong", "AlpcDataViewAttr *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcCreateSectionView",
                "NtStatus",
                ("void *", "Ulong", "AlpcDataViewAttr *"),
                nodiscard=True,
            ),
            Symbol(
                "AlpcMaxAllowedMessageLength",
                "Ulong",
                (),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcQueryInformationMessage",
                "NtStatus",
                (
                    "void *",
                    "PortMessage *",
                    "AlpcMessageInformationClass",
                    "void *",
                    "Ulong",
                    "Ulong *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcQueryInformationMessage",
                "NtStatus",
                (
                    "void *",
                    "PortMessage *",
                    "AlpcMessageInformationClass",
                    "void *",
                    "Ulong",
                    "Ulong *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcCreateSecurityContext",
                "NtStatus",
                ("void *", "Ulong", "AlpcSecurityAttr *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcCreateSecurityContext",
                "NtStatus",
                ("void *", "Ulong", "AlpcSecurityAttr *"),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcOpenSenderThread",
                "NtStatus",
                (
                    "Handle *",
                    "void *",
                    "PortMessage *",
                    "Ulong",
                    "Ulong",
                    "ObjectAttributes *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcOpenSenderThread",
                "NtStatus",
                (
                    "Handle *",
                    "void *",
                    "PortMessage *",
                    "Ulong",
                    "Ulong",
                    "ObjectAttributes *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtTerminateProcess",
                "NtStatus",
                ("void *", "Long"),
                nodiscard=True,
            ),
            Symbol(
                "ZwTerminateProcess",
                "NtStatus",
                ("void *", "Long"),
                nodiscard=True,
            ),
            Symbol(
                "NtDeviceIoControlFile",
                "NtStatus",
                (
                    "void *",
                    "void *",
                    "IoApcRoutine",
                    "void *",
                    "IoStatusBlock *",
                    "Ulong",
                    "void *",
                    "Ulong",
                    "void *",
                    "Ulong",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwDeviceIoControlFile",
                "NtStatus",
                (
                    "void *",
                    "void *",
                    "IoApcRoutine",
                    "void *",
                    "IoStatusBlock *",
                    "Ulong",
                    "void *",
                    "Ulong",
                    "void *",
                    "Ulong",
                ),
                nodiscard=True,
            ),
            Symbol(
                "RtlAcquirePrivilege",
                "NtStatus",
                ("Ulong *", "Ulong", "Ulong", "void **"),
                nodiscard=True,
            ),
            Symbol(
                "RtlReleasePrivilege",
                "void",
                ("void *",),
            ),
            Symbol(
                "RtlQueryEnvironmentVariable_U",
                "NtStatus",
                ("void *", "UnicodeString const *", "UnicodeString *"),
                nodiscard=True,
            ),
            Symbol(
                "NtSetInformationVirtualMemory",
                "NtStatus",
                (
                    "void *",
                    "VirtualMemoryInformationClass",
                    "::std::size_t",
                    "MemoryRangeEntry *",
                    "void *",
                    "Ulong",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwSetInformationVirtualMemory",
                "NtStatus",
                (
                    "void *",
                    "VirtualMemoryInformationClass",
                    "::std::size_t",
                    "MemoryRangeEntry *",
                    "void *",
                    "Ulong",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtDelayExecution",
                "NtStatus",
                ("Boolean", "Long64 *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwDelayExecution",
                "NtStatus",
                ("Boolean", "Long64 *"),
                nodiscard=True,
            ),
            Symbol(
                "NtYieldExecution",
                "NtStatus",
                (),
                nodiscard=True,
            ),
            Symbol(
                "ZwYieldExecution",
                "NtStatus",
                (),
                nodiscard=True,
            ),
            Symbol(
                "NtSetTimerResolution",
                "NtStatus",
                ("Ulong", "Boolean", "Ulong *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwSetTimerResolution",
                "NtStatus",
                ("Ulong", "Boolean", "Ulong *"),
                nodiscard=True,
            ),
            Symbol(
                "NtCreateTimer",
                "NtStatus",
                ("Handle *", "Ulong", "ObjectAttributes const *", "TimerType"),
                nodiscard=True,
            ),
            Symbol(
                "ZwCreateTimer",
                "NtStatus",
                ("Handle *", "Ulong", "ObjectAttributes const *", "TimerType"),
                nodiscard=True,
            ),
            Symbol(
                "NtSetTimer",
                "NtStatus",
                (
                    "void *",
                    "Long64 *",
                    "TimerApcRoutine",
                    "void *",
                    "Boolean",
                    "Long",
                    "Boolean *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ZwSetTimer",
                "NtStatus",
                (
                    "void *",
                    "Long64 *",
                    "TimerApcRoutine",
                    "void *",
                    "Boolean",
                    "Long",
                    "Boolean *",
                ),
                nodiscard=True,
            ),
            Symbol(
                "NtCancelTimer",
                "NtStatus",
                ("void *", "Boolean *"),
                nodiscard=True,
            ),
            Symbol(
                "ZwCancelTimer",
                "NtStatus",
                ("void *", "Boolean *"),
                nodiscard=True,
            ),
        ),
    ),
    Library(
        api="win32",
        name="kernel32",
        namespace="pltxt2htm::details::symbols::win32",
        definitions_header="win32/definitions.hh",
        symbols=(
            Symbol(
                "LoadLibraryExW",
                "ModuleHandle",
                ("WideChar const*", "Handle", "Dword"),
                nodiscard=True,
            ),
            Symbol(
                "GetProcAddress",
                "Procedure",
                ("ModuleHandle", "Char const*"),
                nodiscard=True,
            ),
            Symbol(
                "FreeLibrary",
                "Bool",
                ("ModuleHandle",),
                nodiscard=True,
            ),
            Symbol("LoadLibraryA", "ModuleHandle", ("Char const*",), nodiscard=True),
            Symbol(
                "LoadLibraryW", "ModuleHandle", ("WideChar const*",), nodiscard=True
            ),
            Symbol(
                "LoadLibraryExA",
                "ModuleHandle",
                ("Char const*", "Handle", "Dword"),
                nodiscard=True,
            ),
            Symbol(
                "CreateFileMappingA",
                "Handle",
                (
                    "Handle",
                    "SecurityAttributes*",
                    "Dword",
                    "Dword",
                    "Dword",
                    "Char const*",
                ),
                nodiscard=True,
            ),
            Symbol(
                "CreateFileMappingW",
                "Handle",
                (
                    "Handle",
                    "SecurityAttributes*",
                    "Dword",
                    "Dword",
                    "Dword",
                    "WideChar const*",
                ),
                nodiscard=True,
            ),
            Symbol(
                "MapViewOfFile",
                "void*",
                ("Handle", "Dword", "Dword", "Dword", "::std::size_t"),
                nodiscard=True,
            ),
            Symbol("GetStdHandle", "Handle", ("Dword",), nodiscard=True),
            Symbol(
                "GetModuleHandleA", "ModuleHandle", ("Char const*",), nodiscard=True
            ),
            Symbol(
                "GetModuleHandleW", "ModuleHandle", ("WideChar const*",), nodiscard=True
            ),
            Symbol("GetLastError", "Dword", (), nodiscard=True),
            Symbol(
                "FormatMessageA",
                "Dword",
                ("Dword", "void const*", "Dword", "Dword", "Char*", "Dword", "Char**"),
                nodiscard=True,
            ),
            Symbol(
                "FormatMessageW",
                "Dword",
                (
                    "Dword",
                    "void const*",
                    "Dword",
                    "Dword",
                    "WideChar*",
                    "Dword",
                    "Char**",
                ),
                nodiscard=True,
            ),
            Symbol("SetEndOfFile", "Bool", ("Handle",), nodiscard=True),
            Symbol("UnmapViewOfFile", "Bool", ("void const*",), nodiscard=True),
            Symbol(
                "WriteFile",
                "Bool",
                ("Handle", "void const*", "Dword", "Dword*", "Overlapped*"),
                nodiscard=True,
            ),
            Symbol(
                "ReadFile",
                "Bool",
                ("Handle", "void*", "Dword", "Dword*", "Overlapped*"),
                nodiscard=True,
            ),
            Symbol(
                "SetFilePointer",
                "Dword",
                ("Handle", "Long", "Long*", "Dword"),
                nodiscard=True,
            ),
            Symbol(
                "SetFilePointerEx",
                "Bool",
                ("Handle", "Long64", "Long64*", "Dword"),
                nodiscard=True,
            ),
            Symbol(
                "DuplicateHandle",
                "Bool",
                ("Handle", "Handle", "Handle", "Handle*", "Dword", "int", "Dword"),
                nodiscard=True,
            ),
            Symbol(
                "CreatePipe",
                "Bool",
                ("Handle*", "Handle*", "SecurityAttributes*", "Dword"),
                nodiscard=True,
            ),
            Symbol("WaitForSingleObject", "Dword", ("Handle", "Dword"), nodiscard=True),
            Symbol("CancelIo", "Bool", ("Handle",), nodiscard=True),
            Symbol(
                "GetFileInformationByHandle",
                "Bool",
                ("Handle", "ByHandleFileInformation*"),
                nodiscard=True,
            ),
            Symbol(
                "GetUserDefaultLocaleName", "int", ("WideChar*", "int"), nodiscard=True
            ),
            Symbol("GetUserDefaultLCID", "Dword", (), nodiscard=True),
            Symbol(
                "GetSystemTimePreciseAsFileTime",
                "void",
                ("FileTime*",),
                nodiscard=False,
            ),
            Symbol("GetSystemTimeAsFileTime", "void", ("FileTime*",), nodiscard=False),
            Symbol("QueryUnbiasedInterruptTime", "Bool", ("Ulong64*",), nodiscard=True),
            Symbol("QueryPerformanceCounter", "Bool", ("Long64*",), nodiscard=True),
            Symbol("QueryPerformanceFrequency", "Bool", ("Long64*",), nodiscard=True),
            Symbol(
                "GetProcessTimes",
                "Bool",
                ("Handle", "FileTime*", "FileTime*", "FileTime*", "FileTime*"),
                nodiscard=True,
            ),
            Symbol(
                "GetThreadTimes",
                "Bool",
                ("Handle", "FileTime*", "FileTime*", "FileTime*", "FileTime*"),
                nodiscard=True,
            ),
            Symbol(
                "GetHandleInformation", "Bool", ("Handle", "Dword*"), nodiscard=True
            ),
            Symbol(
                "SetHandleInformation",
                "Bool",
                ("Handle", "Dword", "Dword"),
                nodiscard=True,
            ),
            Symbol("GetTempPathA", "Dword", ("Dword", "Char*"), nodiscard=True),
            Symbol("GetTempPathW", "Dword", ("Dword", "WideChar*"), nodiscard=True),
            Symbol("CloseHandle", "Bool", ("Handle",), nodiscard=True),
            Symbol(
                "LockFileEx",
                "Bool",
                ("Handle", "Dword", "Dword", "Dword", "Dword", "Overlapped*"),
                nodiscard=True,
            ),
            Symbol(
                "UnlockFileEx",
                "Bool",
                ("Handle", "Dword", "Dword", "Dword", "Overlapped*"),
                nodiscard=True,
            ),
            Symbol(
                "DeviceIoControl",
                "Bool",
                (
                    "Handle",
                    "Dword",
                    "void*",
                    "Dword",
                    "void*",
                    "Dword",
                    "Dword*",
                    "Overlapped*",
                ),
                nodiscard=True,
            ),
            Symbol("GetFileType", "Dword", ("Handle",), nodiscard=True),
            Symbol("GetACP", "Uint", (), nodiscard=True),
            Symbol(
                "GetEnvironmentVariableA",
                "Dword",
                ("Char const*", "Char*", "Dword"),
                nodiscard=True,
            ),
            Symbol(
                "GetEnvironmentVariableW",
                "Dword",
                ("WideChar const*", "WideChar*", "Dword"),
                nodiscard=True,
            ),
            Symbol("GetConsoleMode", "Bool", ("Handle", "Dword*"), nodiscard=True),
            Symbol("SetConsoleMode", "Bool", ("Handle", "Dword"), nodiscard=True),
            Symbol(
                "ReadConsoleA",
                "Bool",
                ("Handle", "void*", "Dword", "Dword*", "void*"),
                nodiscard=True,
            ),
            Symbol(
                "ReadConsoleW",
                "Bool",
                ("Handle", "void*", "Dword", "Dword*", "void*"),
                nodiscard=True,
            ),
            Symbol(
                "WriteConsoleA",
                "Bool",
                ("Handle", "void const*", "Dword", "Dword*", "void*"),
                nodiscard=True,
            ),
            Symbol(
                "WriteConsoleW",
                "Bool",
                ("Handle", "void const*", "Dword", "Dword*", "void*"),
                nodiscard=True,
            ),
            Symbol(
                "GetConsoleScreenBufferInfo",
                "Bool",
                ("Handle", "ConsoleScreenBufferInfo*"),
                nodiscard=True,
            ),
            Symbol(
                "ScrollConsoleScreenBufferA",
                "Bool",
                (
                    "Handle",
                    "SmallRect const*",
                    "SmallRect const*",
                    "Coord",
                    "CharInfo const*",
                ),
                nodiscard=True,
            ),
            Symbol(
                "ScrollConsoleScreenBufferW",
                "Bool",
                (
                    "Handle",
                    "SmallRect const*",
                    "SmallRect const*",
                    "Coord",
                    "CharInfo const*",
                ),
                nodiscard=True,
            ),
            Symbol(
                "SetConsoleCursorPosition", "Bool", ("Handle", "Coord"), nodiscard=True
            ),
            Symbol("InitializeCriticalSection", "void", ("void*",), nodiscard=False),
            Symbol("EnterCriticalSection", "void", ("void*",), nodiscard=False),
            Symbol("TryEnterCriticalSection", "Bool", ("void*",), nodiscard=True),
            Symbol("LeaveCriticalSection", "void", ("void*",), nodiscard=False),
            Symbol("DeleteCriticalSection", "void", ("void*",), nodiscard=False),
            Symbol("GetCurrentProcessId", "Dword", (), nodiscard=True),
            Symbol("FlushFileBuffers", "Bool", ("Handle",), nodiscard=True),
            Symbol(
                "GetQueuedCompletionStatus",
                "Bool",
                ("Handle", "Dword*", "::std::size_t*", "Overlapped**", "Dword"),
                nodiscard=True,
            ),
            Symbol(
                "GetTimeZoneInformation",
                "Dword",
                ("TimeZoneInformation*",),
                nodiscard=True,
            ),
            Symbol("SetConsoleCP", "Bool", ("Uint",), nodiscard=True),
            Symbol("SetConsoleOutputCP", "Bool", ("Uint",), nodiscard=True),
            Symbol("GetConsoleCP", "Uint", (), nodiscard=True),
            Symbol("GetConsoleOutputCP", "Uint", (), nodiscard=True),
            Symbol("GetSystemInfo", "void", ("SystemInfo*",), nodiscard=False),
            Symbol(
                "SetFileInformationByHandle",
                "Bool",
                ("Handle", "FileInfoByHandleClass", "void*", "Dword"),
                nodiscard=True,
            ),
            Symbol(
                "SetFileTime",
                "Bool",
                ("Handle", "FileTime const*", "FileTime const*", "FileTime const*"),
                nodiscard=True,
            ),
            Symbol("AcquireSRWLockExclusive", "void", ("void*",), nodiscard=False),
            Symbol("TryAcquireSRWLockExclusive", "Boolean", ("void*",), nodiscard=True),
            Symbol("ReleaseSRWLockExclusive", "void", ("void*",), nodiscard=False),
            Symbol(
                "CreateProcessW",
                "Bool",
                (
                    "WideChar const*",
                    "WideChar*",
                    "SecurityAttributes*",
                    "SecurityAttributes*",
                    "int",
                    "Dword",
                    "void*",
                    "WideChar const*",
                    "StartupInfoW*",
                    "ProcessInformation*",
                ),
                nodiscard=True,
            ),
            Symbol(
                "CreateProcessA",
                "Bool",
                (
                    "Char const*",
                    "Char*",
                    "SecurityAttributes*",
                    "SecurityAttributes*",
                    "int",
                    "Dword",
                    "void*",
                    "Char const*",
                    "StartupInfoA*",
                    "ProcessInformation*",
                ),
                nodiscard=True,
            ),
            Symbol(
                "K32GetMappedFileNameW",
                "Dword",
                ("Handle", "void*", "WideChar*", "Dword"),
                nodiscard=True,
            ),
            Symbol(
                "K32GetMappedFileNameA",
                "Dword",
                ("Handle", "void*", "Char*", "Dword"),
                nodiscard=True,
            ),
            Symbol("GetExitCodeProcess", "Bool", ("Handle", "Dword*"), nodiscard=True),
            Symbol(
                "QueryDosDeviceW",
                "Dword",
                ("WideChar const*", "WideChar*", "Dword"),
                nodiscard=True,
            ),
            Symbol(
                "QueryDosDeviceA",
                "Dword",
                ("Char const*", "Char*", "Dword"),
                nodiscard=True,
            ),
            Symbol(
                "FindNextFileW", "Bool", ("Handle", "Win32FindDataW*"), nodiscard=True
            ),
            Symbol(
                "FindNextFileA", "Bool", ("Handle", "Win32FindDataA*"), nodiscard=True
            ),
            Symbol("FindClose", "Bool", ("Handle",), nodiscard=True),
            Symbol("DeleteFileW", "Bool", ("WideChar const*",), nodiscard=True),
            Symbol("DeleteFileA", "Bool", ("Char const*",), nodiscard=True),
            Symbol("RemoveDirectoryW", "Bool", ("WideChar const*",), nodiscard=True),
            Symbol("RemoveDirectoryA", "Bool", ("Char const*",), nodiscard=True),
            Symbol(
                "CreateDirectoryW",
                "Bool",
                ("WideChar const*", "SecurityAttributes*"),
                nodiscard=True,
            ),
            Symbol(
                "CreateDirectoryA",
                "Bool",
                ("Char const*", "SecurityAttributes*"),
                nodiscard=True,
            ),
            Symbol(
                "SetFileAttributesW",
                "Bool",
                ("WideChar const*", "Dword"),
                nodiscard=True,
            ),
            Symbol(
                "SetFileAttributesA", "Bool", ("Char const*", "Dword"), nodiscard=True
            ),
            Symbol("GetFileAttributesW", "Dword", ("WideChar const*",), nodiscard=True),
            Symbol("GetFileAttributesA", "Dword", ("Char const*",), nodiscard=True),
            Symbol(
                "CopyFileW",
                "Bool",
                ("WideChar const*", "WideChar const*", "int"),
                nodiscard=True,
            ),
            Symbol(
                "CopyFileA",
                "Bool",
                ("Char const*", "Char const*", "int"),
                nodiscard=True,
            ),
            Symbol(
                "MoveFileW",
                "Bool",
                ("WideChar const*", "WideChar const*"),
                nodiscard=True,
            ),
            Symbol("MoveFileA", "Bool", ("Char const*", "Char const*"), nodiscard=True),
            Symbol("TerminateProcess", "Bool", ("Handle", "Uint"), nodiscard=True),
            Symbol(
                "GetFinalPathNameByHandleW",
                "Dword",
                ("Handle", "WideChar*", "Dword", "Dword"),
                nodiscard=True,
            ),
            Symbol(
                "GetFinalPathNameByHandleA",
                "Dword",
                ("Handle", "Char*", "Dword", "Dword"),
                nodiscard=True,
            ),
            Symbol("GetProcessId", "Dword", ("Handle",), nodiscard=True),
            Symbol(
                "ConnectNamedPipe", "Bool", ("Handle", "Overlapped*"), nodiscard=True
            ),
            Symbol("DisconnectNamedPipe", "Bool", ("Handle",), nodiscard=True),
            Symbol(
                "SetWaitableTimer",
                "Bool",
                ("Handle", "Long64 const*", "Long", "TimerApcRoutine", "void*", "int"),
                nodiscard=True,
            ),
            Symbol("CancelWaitableTimer", "Bool", ("Handle",), nodiscard=True),
            Symbol(
                "PrefetchVirtualMemory",
                "Bool",
                ("Handle", "::std::size_t", "MemoryRangeEntry*", "Dword"),
                nodiscard=True,
            ),
            Symbol(
                "SetConsoleTextAttribute", "Bool", ("Handle", "Word"), nodiscard=True
            ),
            Symbol("GetCurrentThreadId", "Dword", (), nodiscard=True),
            Symbol("Sleep", "void", ("Dword",), nodiscard=False),
            Symbol("SwitchToThread", "Bool", (), nodiscard=True),
            Symbol("FreeEnvironmentStringsW", "Bool", ("WideChar*",), nodiscard=True),
            Symbol("FreeEnvironmentStringsA", "Bool", ("Char*",), nodiscard=True),
            Symbol(
                "GetFullPathNameW",
                "Dword",
                ("WideChar const*", "Dword", "WideChar*", "WideChar**"),
                nodiscard=True,
            ),
            Symbol(
                "GetFullPathNameA",
                "Dword",
                ("Char const*", "Dword", "Char*", "Char**"),
                nodiscard=True,
            ),
            Symbol(
                "GetVolumeInformationW",
                "Bool",
                (
                    "WideChar const*",
                    "WideChar*",
                    "Dword",
                    "Dword*",
                    "Dword*",
                    "Dword*",
                    "WideChar*",
                    "Dword",
                ),
                nodiscard=True,
            ),
            Symbol(
                "GetVolumeInformationA",
                "Bool",
                (
                    "Char const*",
                    "Char*",
                    "Dword",
                    "Dword*",
                    "Dword*",
                    "Dword*",
                    "Char*",
                    "Dword",
                ),
                nodiscard=True,
            ),
            Symbol(
                "GetDiskFreeSpaceW",
                "Bool",
                ("WideChar const*", "Dword*", "Dword*", "Dword*", "Dword*"),
                nodiscard=True,
            ),
            Symbol(
                "GetDiskFreeSpaceA",
                "Bool",
                ("Char const*", "Dword*", "Dword*", "Dword*", "Dword*"),
                nodiscard=True,
            ),
            Symbol(
                "CreateFileA",
                "Handle",
                (
                    "Char const*",
                    "Dword",
                    "Dword",
                    "SecurityAttributes*",
                    "Dword",
                    "Dword",
                    "Handle",
                ),
                nodiscard=True,
            ),
            Symbol(
                "CreateFileW",
                "Handle",
                (
                    "WideChar const*",
                    "Dword",
                    "Dword",
                    "SecurityAttributes*",
                    "Dword",
                    "Dword",
                    "Handle",
                ),
                nodiscard=True,
            ),
            Symbol(
                "CreateIoCompletionPort",
                "Handle",
                ("Handle", "Handle", "::std::size_t", "Dword"),
                nodiscard=True,
            ),
            Symbol(
                "FindFirstFileA",
                "Handle",
                ("Char const*", "Win32FindDataA*"),
                nodiscard=True,
            ),
            Symbol(
                "FindFirstFileW",
                "Handle",
                ("WideChar const*", "Win32FindDataW*"),
                nodiscard=True,
            ),
            Symbol(
                "CreateNamedPipeA",
                "Handle",
                (
                    "Char const*",
                    "Dword",
                    "Dword",
                    "Dword",
                    "Dword",
                    "Dword",
                    "Dword",
                    "SecurityAttributes*",
                ),
                nodiscard=True,
            ),
            Symbol(
                "CreateNamedPipeW",
                "Handle",
                (
                    "WideChar const*",
                    "Dword",
                    "Dword",
                    "Dword",
                    "Dword",
                    "Dword",
                    "Dword",
                    "SecurityAttributes*",
                ),
                nodiscard=True,
            ),
            Symbol(
                "CreateWaitableTimerA",
                "Handle",
                ("SecurityAttributes*", "Bool", "Char const*"),
                nodiscard=True,
            ),
            Symbol(
                "CreateWaitableTimerW",
                "Handle",
                ("SecurityAttributes*", "Bool", "WideChar const*"),
                nodiscard=True,
            ),
            Symbol(
                "CreateThread",
                "Handle",
                (
                    "SecurityAttributes*",
                    "::std::size_t",
                    "ThreadStartRoutine",
                    "void*",
                    "Dword",
                    "Dword*",
                ),
                nodiscard=True,
            ),
            Symbol("GetEnvironmentStringsA", "Char*", (), nodiscard=True),
            Symbol("GetEnvironmentStringsW", "WideChar*", (), nodiscard=True),
            Symbol("LocalFree", "Handle", ("Handle",), nodiscard=True),
            Symbol(
                "AddVectoredExceptionHandler",
                "Handle",
                ("Dword", "VectoredExceptionHandler"),
                nodiscard=True,
            ),
            Symbol(
                "RaiseException",
                "void",
                ("Dword", "Dword", "Dword", "::std::size_t const*"),
                nodiscard=False,
            ),
            Symbol("ExitProcess", "void", ("Dword",), nodiscard=False),
        ),
    ),
    Library(
        api="win32",
        name="dbghelp",
        namespace="pltxt2htm::details::symbols::win32",
        definitions_header="win32/dbghelp_definitions.hh",
        symbols=(
            Symbol(
                "SymInitialize",
                "Bool",
                ("Handle", "Char const*", "Bool"),
                nodiscard=True,
            ),
            Symbol(
                "SymRefreshModuleList",
                "Bool",
                ("Handle",),
                nodiscard=True,
            ),
            Symbol(
                "SymCleanup",
                "Bool",
                ("Handle",),
                nodiscard=True,
            ),
            Symbol(
                "SymFromAddr",
                "Bool",
                ("Handle", "Dword64", "Dword64*", "SymbolInfo*"),
                nodiscard=True,
            ),
            Symbol(
                "SymGetLineFromAddr64",
                "Bool",
                ("Handle", "Dword64", "Dword*", "ImageHlpLine64*"),
                nodiscard=True,
            ),
        ),
    ),
)


MSVC_TARGETS = (
    ("i686", "i686-pc-windows-msvc"),
    ("x64", "x86_64-pc-windows-msvc"),
    ("arm32", "armv7-pc-windows-msvc"),
    ("arm64", "aarch64-pc-windows-msvc"),
    ("arm64ec", "arm64ec-pc-windows-msvc"),
)


GENERATED_NOTICE = "// Generated by scripts/gen_windows_symbols.py. Do not edit.\n"


def format_parameters(parameters: tuple[str, ...]) -> str:
    return ", ".join(parameters)


def declaration(symbol: Symbol, x86_stdcall_stack_bytes: dict[str, int]) -> str:
    attribute = "[[nodiscard]]\n" if symbol.nodiscard else ""
    if symbol.calling_convention == "stdcall":
        convention = "PLTXT2HTM_DETAIL_STDCALL"
        suffix = (
            f" PLTXT2HTM_DETAIL_STDCALL_ASM_NAME({symbol.export_name}, "
            f"{x86_stdcall_stack_bytes[symbol.export_name]})"
        )
    elif symbol.calling_convention == "cdecl":
        convention = "PLTXT2HTM_DETAIL_CDECL"
        suffix = f" PLTXT2HTM_DETAIL_CDECL_ASM_NAME({symbol.export_name})"
    else:
        raise ValueError(f"unsupported calling convention: {symbol.calling_convention}")
    return (
        f"{attribute}PLTXT2HTM_DETAIL_DLLIMPORT {symbol.return_type} {convention} {symbol.export_name}"
        f"({format_parameters(symbol.parameters)}) noexcept{suffix};\n"
    )


def generate_declarations(
    library: Library, x86_stdcall_stack_bytes: dict[str, int]
) -> str:
    result = [GENERATED_NOTICE, "\n"]
    result.extend(
        declaration(symbol, x86_stdcall_stack_bytes) for symbol in library.symbols
    )
    return "".join(result)


def local_probe_source(symbol: Symbol, library: Library) -> str:
    result = symbol.return_type
    parameters = format_parameters(symbol.parameters)
    convention = "__stdcall" if symbol.calling_convention == "stdcall" else "__cdecl"
    return f"""#include "{library.definitions_header}"

namespace {library.namespace} {{
__declspec(dllimport) {result} {convention} {symbol.export_name}({parameters}) noexcept;
}}

auto* volatile pltxt2htm_probe = &::{library.namespace}::{symbol.export_name};
"""


def export_probe_source(symbol: Symbol, library: Library) -> str:
    result = symbol.return_type
    parameters = format_parameters(symbol.parameters)
    convention = "__stdcall" if symbol.calling_convention == "stdcall" else "__cdecl"
    return f"""#include "{library.definitions_header}"

namespace {library.namespace} {{
extern \"C\" __declspec(dllimport) {result} {convention} {symbol.export_name}({parameters}) noexcept;
}}

auto* volatile pltxt2htm_probe = &::{library.namespace}::{symbol.export_name};
"""


def undefined_imports(
    source_text: str,
    symbol: Symbol,
    target: str,
    clang: str,
    llvm_nm: str,
    cxx_include_dir: pathlib.Path | None,
) -> tuple[str, ...]:
    compile_command = [clang, f"--target={target}", "-std=c++23"]
    if cxx_include_dir is not None:
        # MSVC's current STL headers reject the ARM32 target, but the probes still need <cstdint>.
        # Use a compatible C++ header set without mixing it with Clang's default C++ headers.
        compile_command.extend(("-nostdinc++", "-isystem", str(cxx_include_dir)))
    compile_command.extend(("-I", str(SYMBOL_ROOT), "-x", "c++", "-c", "-o", "-", "-"))
    compile_result = subprocess.run(
        compile_command,
        input=source_text.encode("ascii"),
        check=False,
        capture_output=True,
    )
    if compile_result.returncode != 0:
        raise RuntimeError(
            f"clang failed for {symbol.export_name} ({target}):\n"
            f"{compile_result.stderr.decode(errors='replace').strip()}"
        )
    nm_result = subprocess.run(
        [llvm_nm, "--undefined-only", "--format=posix", "-"],
        input=compile_result.stdout,
        check=False,
        capture_output=True,
    )
    if nm_result.returncode != 0:
        raise RuntimeError(
            f"llvm-nm failed for {symbol.export_name} ({target}):\n"
            f"{nm_result.stderr.decode(errors='replace').strip()}"
        )
    names = [
        line.split()[0]
        for line in nm_result.stdout.decode("ascii").splitlines()
        if line.strip()
    ]
    imported = tuple(name for name in names if "__imp_" in name)
    if not imported:
        raise RuntimeError(
            f"could not identify import symbol for {symbol.export_name} ({target}); found: {names}"
        )
    return imported


def import_flavor(name: str) -> str:
    return "aux" if "__imp_aux_" in name else "native"


def undefined_symbols(
    symbol: Symbol,
    library: Library,
    target: str,
    clang: str,
    llvm_nm: str,
    cxx_include_dir: pathlib.Path | None,
) -> tuple[tuple[str, str], ...]:
    local = undefined_imports(
        local_probe_source(symbol, library),
        symbol,
        target,
        clang,
        llvm_nm,
        cxx_include_dir,
    )
    exported = undefined_imports(
        export_probe_source(symbol, library),
        symbol,
        target,
        clang,
        llvm_nm,
        cxx_include_dir,
    )
    exported_by_flavor = {import_flavor(name): name for name in exported}
    pairs = tuple((name, exported_by_flavor[import_flavor(name)]) for name in local)
    if len(pairs) != len(exported) or len(exported_by_flavor) != len(exported):
        raise RuntimeError(
            f"could not pair import symbols for {symbol.export_name} ({target}); "
            f"local: {local}; exported: {exported}"
        )
    return pairs


def generate_linker_aliases(
    library: Library,
    target: str,
    clang: str,
    llvm_nm: str,
    cxx_include_dir: pathlib.Path | None,
) -> tuple[str, dict[str, int]]:
    result = [GENERATED_NOTICE, "\n"]
    x86_stdcall_stack_bytes: dict[str, int] = {}
    for symbol in library.symbols:
        pairs = undefined_symbols(
            symbol, library, target, clang, llvm_nm, cxx_include_dir
        )
        for local, exported in pairs:
            result.append(
                f'#pragma comment(linker, "/alternatename:{local}={exported}")\n'
            )
        if target.startswith("i686-") and symbol.calling_convention == "stdcall":
            match = re.search(r"@(\d+)$", pairs[0][1])
            if match is None:
                raise RuntimeError(
                    f"missing i686 stdcall decoration for {symbol.export_name}: {pairs[0][1]}"
                )
            x86_stdcall_stack_bytes[symbol.export_name] = int(match.group(1))
    return "".join(result), x86_stdcall_stack_bytes


def generated_files(
    clang: str,
    llvm_nm: str,
    cxx_include_dir: pathlib.Path | None,
    library_name: str | None = None,
) -> dict[pathlib.Path, str]:
    result: dict[pathlib.Path, str] = {}
    for library in LIBRARIES:
        if library_name is not None and library.name != library_name:
            continue
        directory = SYMBOL_ROOT / library.api
        i686_stdcall_stack_bytes: dict[str, int] = {}
        for architecture, target in MSVC_TARGETS:
            aliases, target_stack_bytes = generate_linker_aliases(
                library, target, clang, llvm_nm, cxx_include_dir
            )
            result[directory / f"{library.name}_msvc_linker_{architecture}.inc"] = (
                aliases
            )
            i686_stdcall_stack_bytes.update(target_stack_bytes)
        result[directory / f"{library.name}.inc"] = generate_declarations(
            library, i686_stdcall_stack_bytes
        )
    return result


def resolve_tool(value: str, description: str) -> str:
    resolved = shutil.which(value)
    if resolved is None:
        raise RuntimeError(f"{description} was not found: {value}")
    return resolved


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--check", action="store_true", help="fail if generated files are stale"
    )
    parser.add_argument(
        "--library",
        choices=[library.name for library in LIBRARIES],
        help="generate one library",
    )
    parser.add_argument("--clang", default="clang++", help="Clang C++ driver")
    parser.add_argument("--llvm-nm", default="llvm-nm", help="LLVM symbol table dumper")
    parser.add_argument(
        "--cxx-include-dir",
        type=pathlib.Path,
        help="C++ standard library header directory for cross-compiling",
    )
    arguments = parser.parse_args()

    clang = resolve_tool(arguments.clang, "Clang C++ driver")
    llvm_nm = resolve_tool(arguments.llvm_nm, "llvm-nm")
    files = generated_files(
        clang, llvm_nm, arguments.cxx_include_dir, arguments.library
    )
    stale: list[pathlib.Path] = []
    for path, content in files.items():
        if arguments.check:
            if not path.exists() or path.read_text(encoding="ascii") != content:
                stale.append(path.relative_to(ROOT))
            continue
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(content, encoding="ascii", newline="\n")

    if stale:
        print("Generated Windows symbol files are stale:")
        for path in stale:
            print(f"  {path}")
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
