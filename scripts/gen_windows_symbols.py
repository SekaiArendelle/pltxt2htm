#!/usr/bin/env python3
"""Generate Windows API declarations and MSVC import-symbol aliases."""

from __future__ import annotations

import argparse
import dataclasses
import pathlib
import re
import shutil
import subprocess


ROOT = pathlib.Path(__file__).resolve().parents[1]
SYMBOL_ROOT = ROOT / "include" / "pltxt2htm" / "details" / "symbols"


@dataclasses.dataclass(frozen=True)
class Symbol:
    export_name: str
    return_type: str
    parameters: tuple[str, ...]
    nodiscard: bool = False
    calling_convention: str = "stdcall"


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
                "Ulong",
                ('void *', 'void *', 'void *', 'void **', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', ),
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
                "Ulong",
                ('void *', ),
                nodiscard=True,
            ),
            Symbol(
                "RtlNtStatusToDosError",
                "Ulong",
                ('::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "NtCreateFile",
                "Ulong",
                ('void **', '::std::uint_least32_t', 'object_attributes *', 'io_status_block *', '::std::int_least64_t *', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', 'void *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwCreateFile",
                "Ulong",
                ('void **', '::std::uint_least32_t', 'object_attributes *', 'io_status_block *', '::std::int_least64_t *', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', 'void *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "NtCreateSection",
                "Ulong",
                ('void **', '::std::uint_least32_t', 'object_attributes *', '::std::uint_least64_t *', '::std::uint_least32_t', '::std::uint_least32_t', 'void *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwCreateSection",
                "Ulong",
                ('void **', '::std::uint_least32_t', 'object_attributes *', '::std::uint_least64_t *', '::std::uint_least32_t', '::std::uint_least32_t', 'void *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtQueryInformationProcess",
                "Ulong",
                ('void *', 'process_information_class', 'void *', '::std::uint_least32_t', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwQueryInformationProcess",
                "Ulong",
                ('void *', 'process_information_class', 'void *', '::std::uint_least32_t', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtWriteFile",
                "Ulong",
                ('void *', 'void *', 'pio_apc_routine', 'void *', 'io_status_block *', 'void const *', '::std::uint_least32_t', '::std::int_least64_t *', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwWriteFile",
                "Ulong",
                ('void *', 'void *', 'pio_apc_routine', 'void *', 'io_status_block *', 'void const *', '::std::uint_least32_t', '::std::int_least64_t *', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtReadFile",
                "Ulong",
                ('void *', 'void *', 'pio_apc_routine', 'void *', 'io_status_block *', 'void *', '::std::uint_least32_t', '::std::int_least64_t *', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwReadFile",
                "Ulong",
                ('void *', 'void *', 'pio_apc_routine', 'void *', 'io_status_block *', 'void *', '::std::uint_least32_t', '::std::int_least64_t *', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtQueryObject",
                "Ulong",
                ('void *', 'object_information_class', 'void *', '::std::uint_least32_t', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwQueryObject",
                "Ulong",
                ('void *', 'object_information_class', 'void *', '::std::uint_least32_t', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtQueryDirectoryFile",
                "Ulong",
                ('void *', 'void *', 'pio_apc_routine', 'void *', 'io_status_block *', 'void *', '::std::uint_least32_t', 'file_information_class', 'int', 'unicode_string *', 'int', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwQueryDirectoryFile",
                "Ulong",
                ('void *', 'void *', 'pio_apc_routine', 'void *', 'io_status_block *', 'void *', '::std::uint_least32_t', 'file_information_class', 'int', 'unicode_string *', 'int', ),
                nodiscard=True,
            ),
            Symbol(
                "NtQuerySection",
                "Ulong",
                ('void *', 'section_information_class', 'void *', '::std::size_t', '::std::size_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwQuerySection",
                "Ulong",
                ('void *', 'section_information_class', 'void *', '::std::size_t', '::std::size_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtQueryInformationFile",
                "Ulong",
                ('void *', 'io_status_block *', 'void *', '::std::uint_least32_t', 'file_information_class', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwQueryInformationFile",
                "Ulong",
                ('void *', 'io_status_block *', 'void *', '::std::uint_least32_t', 'file_information_class', ),
                nodiscard=True,
            ),
            Symbol(
                "NtSetInformationFile",
                "Ulong",
                ('void *', 'io_status_block *', 'void *', '::std::uint_least32_t', 'file_information_class', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwSetInformationFile",
                "Ulong",
                ('void *', 'io_status_block *', 'void *', '::std::uint_least32_t', 'file_information_class', ),
                nodiscard=True,
            ),
            Symbol(
                "NtWaitForSingleObject",
                "Ulong",
                ('void *', 'int', '::std::uint_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwWaitForSingleObject",
                "Ulong",
                ('void *', 'int', '::std::uint_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtWaitForMultipleObjects",
                "Ulong",
                ('::std::uint_least32_t', 'void *const *', 'wait_type', 'int', '::std::uint_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwWaitForMultipleObjects",
                "Ulong",
                ('::std::uint_least32_t', 'void *const *', 'wait_type', 'int', '::std::uint_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtSetSystemTime",
                "Ulong",
                ('::std::int_least64_t *', '::std::int_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwSetSystemTime",
                "Ulong",
                ('::std::int_least64_t *', '::std::int_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtCreateProcess",
                "Ulong",
                ('void **', '::std::uint_least32_t', 'object_attributes *', 'void *', '::std::uint_least32_t', 'void *', 'void *', 'void *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwCreateProcess",
                "Ulong",
                ('void **', '::std::uint_least32_t', 'object_attributes *', 'void *', '::std::uint_least32_t', 'void *', 'void *', 'void *', ),
                nodiscard=True,
            ),
            Symbol(
                "RtlDosPathNameToNtPathName_U",
                "char unsigned",
                ('char16_t const *', 'unicode_string *', 'char16_t const **', 'rtl_relative_name_u *', ),
                nodiscard=True,
            ),
            Symbol(
                "RtlDosPathNameToNtPathName_U_WithStatus",
                "Ulong",
                ('char16_t const *', 'unicode_string *', 'char16_t const **', 'rtl_relative_name_u *', ),
                nodiscard=True,
            ),
            Symbol(
                "RtlNtPathNameToDosPathName",
                "Ulong",
                ('::std::uint_least32_t', 'rtl_unicode_string_buffer *', '::std::uint_least32_t *', 'char16_t **', ),
                nodiscard=True,
            ),
            Symbol(
                "RtlFreeUnicodeString",
                "void",
                ('unicode_string *', ),
            ),
            Symbol(
                "RtlInitializeCriticalSection",
                "void",
                ('void *', ),
            ),
            Symbol(
                "RtlEnterCriticalSection",
                "void",
                ('void *', ),
            ),
            Symbol(
                "RtlTryEnterCriticalSection",
                "int",
                ('void *', ),
                nodiscard=True,
            ),
            Symbol(
                "RtlLeaveCriticalSection",
                "void",
                ('void *', ),
            ),
            Symbol(
                "RtlDeleteCriticalSection",
                "void",
                ('void *', ),
            ),
            Symbol(
                "RtlCreateUserThread",
                "Ulong",
                ('void *', 'void *', 'int', '::std::uint_least32_t', '::std::size_t', '::std::size_t', 'user_thread_start_routine', 'void *', 'void **', 'client_id *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtResumeThread",
                "Ulong",
                ('void *', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwResumeThread",
                "Ulong",
                ('void *', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtLockFile",
                "Ulong",
                ('void *', 'void *', 'pio_apc_routine', 'void *', 'io_status_block *', '::std::int_least64_t *', '::std::int_least64_t *', '::std::uint_least32_t', 'char unsigned', 'char unsigned', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwLockFile",
                "Ulong",
                ('void *', 'void *', 'pio_apc_routine', 'void *', 'io_status_block *', '::std::int_least64_t *', '::std::int_least64_t *', '::std::uint_least32_t', 'char unsigned', 'char unsigned', ),
                nodiscard=True,
            ),
            Symbol(
                "NtUnlockFile",
                "Ulong",
                ('void *', 'io_status_block *', '::std::int_least64_t *', '::std::int_least64_t *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwUnlockFile",
                "Ulong",
                ('void *', 'io_status_block *', '::std::int_least64_t *', '::std::int_least64_t *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "NtFlushBuffersFile",
                "Ulong",
                ('void *', 'io_status_block *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwFlushBuffersFile",
                "Ulong",
                ('void *', 'io_status_block *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtFlushBuffersFileEx",
                "Ulong",
                ('void *', '::std::uint_least32_t', 'void *', '::std::uint_least32_t', 'io_status_block *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwFlushBuffersFileEx",
                "Ulong",
                ('void *', '::std::uint_least32_t', 'void *', '::std::uint_least32_t', 'io_status_block *', ),
                nodiscard=True,
            ),
            Symbol(
                "DbgPrint",
                "Ulong",
                ('char const *', '...', ),
                nodiscard=True,
                calling_convention="cdecl",
            ),
            Symbol(
                "DbgPrintEx",
                "Ulong",
                ('::std::uint_least32_t', '::std::uint_least32_t', 'char const *', '...', ),
                nodiscard=True,
                calling_convention="cdecl",
            ),
            Symbol(
                "RtlCreateProcessParameters",
                "Ulong",
                ('rtl_user_process_parameters **', 'unicode_string *', 'unicode_string *', 'unicode_string *', 'unicode_string *', 'void *', 'unicode_string *', 'unicode_string *', 'unicode_string *', 'unicode_string *', ),
                nodiscard=True,
            ),
            Symbol(
                "RtlCreateProcessParametersEx",
                "Ulong",
                ('rtl_user_process_parameters **', 'unicode_string *', 'unicode_string *', 'unicode_string *', 'unicode_string *', 'void *', 'unicode_string *', 'unicode_string *', 'unicode_string *', 'unicode_string *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "RtlDestroyProcessParameters",
                "Ulong",
                ('rtl_user_process_parameters *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtCreateUserProcess",
                "Ulong",
                ('void *', 'void *', '::std::uint_least32_t', '::std::uint_least32_t', 'object_attributes *', 'object_attributes *', '::std::uint_least32_t', '::std::uint_least32_t', 'rtl_user_process_parameters *', 'ps_create_info *', 'ps_attribute_list *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwCreateUserProcess",
                "Ulong",
                ('void *', 'void *', '::std::uint_least32_t', '::std::uint_least32_t', 'object_attributes *', 'object_attributes *', '::std::uint_least32_t', '::std::uint_least32_t', 'rtl_user_process_parameters *', 'ps_create_info *', 'ps_attribute_list *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtMapViewOfSection",
                "Ulong",
                ('void *', 'void *', 'void **', '::std::size_t', '::std::size_t', 'large_integer const *', '::std::size_t *', 'section_inherit', '::std::uint_least32_t', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwMapViewOfSection",
                "Ulong",
                ('void *', 'void *', 'void **', '::std::size_t', '::std::size_t', 'large_integer const *', '::std::size_t *', 'section_inherit', '::std::uint_least32_t', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "NtUnmapViewOfSection",
                "Ulong",
                ('void *', 'void *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwUnmapViewOfSection",
                "Ulong",
                ('void *', 'void *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtReadVirtualMemory",
                "Ulong",
                ('void *', 'void *', 'void *', '::std::size_t', '::std::size_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwReadVirtualMemory",
                "Ulong",
                ('void *', 'void *', 'void *', '::std::size_t', '::std::size_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtWriteVirtualMemory",
                "Ulong",
                ('void *', 'void *', 'void *', '::std::size_t', '::std::size_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwWriteVirtualMemory",
                "Ulong",
                ('void *', 'void *', 'void *', '::std::size_t', '::std::size_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "RtlAcquirePebLock",
                "void",
                (),
            ),
            Symbol(
                "RtlReleasePebLock",
                "void",
                (),
            ),
            Symbol(
                "NtAllocateVirtualMemory",
                "Ulong",
                ('void *', 'void **', '::std::size_t', '::std::size_t *', '::std::uint_least32_t', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAllocateVirtualMemory",
                "Ulong",
                ('void *', 'void **', '::std::size_t', '::std::size_t *', '::std::uint_least32_t', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "NtFreeVirtualMemory",
                "Ulong",
                ('void *', 'void **', '::std::size_t *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwFreeVirtualMemory",
                "Ulong",
                ('void *', 'void **', '::std::size_t *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "RtlInitUnicodeString",
                "void",
                ('unicode_string *', 'char16_t *', ),
            ),
            Symbol(
                "CsrClientCallServer",
                "Ulong",
                ('void *', 'void *', '::std::uint_least32_t', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "NtQuerySystemInformation",
                "Ulong",
                ('system_information_class', 'void *', '::std::uint_least32_t', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwQuerySystemInformation",
                "Ulong",
                ('system_information_class', 'void *', '::std::uint_least32_t', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtQueryVolumeInformationFile",
                "Ulong",
                ('void *', 'io_status_block *', 'void *', '::std::uint_least32_t', 'fs_information_class', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwQueryVolumeInformationFile",
                "Ulong",
                ('void *', 'io_status_block *', 'void *', '::std::uint_least32_t', 'fs_information_class', ),
                nodiscard=True,
            ),
            Symbol(
                "NtFsControlFile",
                "Ulong",
                ('void *', 'void *', 'pio_apc_routine *', 'void *', 'io_status_block *', '::std::uint_least32_t', 'void *', '::std::uint_least32_t', 'void *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwFsControlFile",
                "Ulong",
                ('void *', 'void *', 'pio_apc_routine *', 'void *', 'io_status_block *', '::std::uint_least32_t', 'void *', '::std::uint_least32_t', 'void *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "NtCreateNamedPipeFile",
                "Ulong",
                ('void **', '::std::uint_least32_t', 'object_attributes *', 'io_status_block *', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', '::std::int_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwCreateNamedPipeFile",
                "Ulong",
                ('void **', '::std::uint_least32_t', 'object_attributes *', 'io_status_block *', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', '::std::uint_least32_t', '::std::int_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtQueryPerformanceCounter",
                "Ulong",
                ('::std::int_least64_t *', '::std::int_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwQueryPerformanceCounter",
                "Ulong",
                ('::std::int_least64_t *', '::std::int_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "RtlGetSystemTimePrecise",
                "::std::uint_least64_t",
                (),
                nodiscard=True,
            ),
            Symbol(
                "NtQueryInformationThread",
                "Ulong",
                ('void *', 'thread_information_class', 'void *', '::std::uint_least32_t', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwQueryInformationThread",
                "Ulong",
                ('void *', 'thread_information_class', 'void *', '::std::uint_least32_t', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "RtlAdjustPrivilege",
                "Ulong",
                ('::std::uint_least32_t', '::std::uint_least8_t', '::std::uint_least8_t', '::std::uint_least8_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtCreateEvent",
                "Ulong",
                ('void **', '::std::uint_least32_t', 'object_attributes *', 'event_type', '::std::uint_least8_t', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwCreateEvent",
                "Ulong",
                ('void **', '::std::uint_least32_t', 'object_attributes *', 'event_type', '::std::uint_least8_t', ),
                nodiscard=True,
            ),
            Symbol(
                "NtSetEvent",
                "Ulong",
                ('void *', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwSetEvent",
                "Ulong",
                ('void *', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtResetEvent",
                "Ulong",
                ('void *', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwResetEvent",
                "Ulong",
                ('void *', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtSetInformationObject",
                "Ulong",
                ('void *', 'object_information_class', 'void *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwSetInformationObject",
                "Ulong",
                ('void *', 'object_information_class', 'void *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "NtQueryAuxiliaryCounterFrequency",
                "Ulong",
                ('::std::uint_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwQueryAuxiliaryCounterFrequency",
                "Ulong",
                ('::std::uint_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "RtlDetermineDosPathNameType_U",
                "rtl_path_type",
                ('char16_t const *', ),
                nodiscard=True,
            ),
            Symbol(
                "RtlGetFullPathName_U",
                "Ulong",
                ('char16_t const *', '::std::uint_least32_t', 'char16_t *', 'char16_t **', ),
                nodiscard=True,
            ),
            Symbol(
                "AlpcInitializeMessageAttribute",
                "Ulong",
                ('::std::uint_least32_t', 'alpc_message_attributes *', '::std::size_t', '::std::size_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "AlpcGetMessageAttribute",
                "void*",
                ("alpc_message_attributes*", "Ulong"),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcCreatePort",
                "Ulong",
                ('void **', 'object_attributes *', 'alpc_port_attributes *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcCreatePort",
                "Ulong",
                ('void **', 'object_attributes *', 'alpc_port_attributes *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcConnectPort",
                "Ulong",
                ('void **', 'unicode_string *', 'object_attributes *', 'alpc_port_attributes *', '::std::uint_least32_t', 'void *', 'port_message *', '::std::size_t *', 'alpc_message_attributes *', 'alpc_message_attributes *', '::std::int_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcConnectPort",
                "Ulong",
                ('void **', 'unicode_string *', 'object_attributes *', 'alpc_port_attributes *', '::std::uint_least32_t', 'void *', 'port_message *', '::std::size_t *', 'alpc_message_attributes *', 'alpc_message_attributes *', '::std::int_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcSendWaitReceivePort",
                "Ulong",
                ('void *', '::std::uint_least32_t', 'port_message *', 'alpc_message_attributes *', 'port_message *', '::std::size_t *', 'alpc_message_attributes *', '::std::int_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcSendWaitReceivePort",
                "Ulong",
                ('void *', '::std::uint_least32_t', 'port_message *', 'alpc_message_attributes *', 'port_message *', '::std::size_t *', 'alpc_message_attributes *', '::std::int_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcAcceptConnectPort",
                "Ulong",
                ('void **', 'void *', '::std::uint_least32_t', 'object_attributes *', 'alpc_port_attributes *', 'void *', 'port_message *', 'alpc_message_attributes *', '::std::uint_least8_t', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcAcceptConnectPort",
                "Ulong",
                ('void **', 'void *', '::std::uint_least32_t', 'object_attributes *', 'alpc_port_attributes *', 'void *', 'port_message *', 'alpc_message_attributes *', '::std::uint_least8_t', ),
                nodiscard=True,
            ),
            Symbol(
                "AlpcGetHeaderSize",
                "::std::size_t",
                ('::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcImpersonateClientOfPort",
                "Ulong",
                ('void *', 'port_message *', 'void *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcImpersonateClientOfPort",
                "Ulong",
                ('void *', 'port_message *', 'void *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcImpersonateClientContainerOfPort",
                "Ulong",
                ('void *', 'port_message *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcImpersonateClientContainerOfPort",
                "Ulong",
                ('void *', 'port_message *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcDisconnectPort",
                "Ulong",
                ('void *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcDisconnectPort",
                "Ulong",
                ('void *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcCreatePortSection",
                "Ulong",
                ('void *', '::std::uint_least32_t', 'void *', '::std::size_t', 'void **', '::std::size_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcCreatePortSection",
                "Ulong",
                ('void *', '::std::uint_least32_t', 'void *', '::std::size_t', 'void **', '::std::size_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcCreateSectionView",
                "Ulong",
                ('void *', '::std::uint_least32_t', 'alpc_data_view_attr *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcCreateSectionView",
                "Ulong",
                ('void *', '::std::uint_least32_t', 'alpc_data_view_attr *', ),
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
                "Ulong",
                ('void *', 'port_message *', 'alpc_message_information_class', 'void *', '::std::size_t', '::std::size_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcQueryInformationMessage",
                "Ulong",
                ('void *', 'port_message *', 'alpc_message_information_class', 'void *', '::std::size_t', '::std::size_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcCreateSecurityContext",
                "Ulong",
                ('void *', '::std::uint_least32_t', 'alpc_security_attr *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcCreateSecurityContext",
                "Ulong",
                ('void *', '::std::uint_least32_t', 'alpc_security_attr *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtAlpcOpenSenderThread",
                "Ulong",
                ('void **', 'void *', 'port_message *', '::std::uint_least32_t', '::std::uint_least32_t', 'object_attributes *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwAlpcOpenSenderThread",
                "Ulong",
                ('void **', 'void *', 'port_message *', '::std::uint_least32_t', '::std::uint_least32_t', 'object_attributes *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtTerminateProcess",
                "Ulong",
                ('void *', '::std::int_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwTerminateProcess",
                "Ulong",
                ('void *', '::std::int_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "NtDeviceIoControlFile",
                "Ulong",
                ('void *', 'void *', 'pio_apc_routine *', 'void *', 'io_status_block *', '::std::uint_least32_t', 'void *', '::std::uint_least32_t', 'void *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwDeviceIoControlFile",
                "Ulong",
                ('void *', 'void *', 'pio_apc_routine *', 'void *', 'io_status_block *', '::std::uint_least32_t', 'void *', '::std::uint_least32_t', 'void *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "RtlAcquirePrivilege",
                "Ulong",
                ('privileges *', '::std::uint_least32_t', '::std::uint_least32_t', 'void **', ),
                nodiscard=True,
            ),
            Symbol(
                "RtlReleasePrivilege",
                "void",
                ('void *', ),
            ),
            Symbol(
                "RtlQueryEnvironmentVariable_U",
                "Ulong",
                ('void *', 'unicode_string *', 'unicode_string *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtSetInformationVirtualMemory",
                "Ulong",
                ('void *', 'virtual_memory_information_class', '::std::size_t', 'memory_range_entry *', 'void *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwSetInformationVirtualMemory",
                "Ulong",
                ('void *', 'virtual_memory_information_class', '::std::size_t', 'memory_range_entry *', 'void *', '::std::uint_least32_t', ),
                nodiscard=True,
            ),
            Symbol(
                "NtDelayExecution",
                "Ulong",
                ('bool', '::std::int_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwDelayExecution",
                "Ulong",
                ('bool', '::std::int_least64_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtYieldExecution",
                "Ulong",
                (),
                nodiscard=True,
            ),
            Symbol(
                "ZwYieldExecution",
                "Ulong",
                (),
                nodiscard=True,
            ),
            Symbol(
                "NtSetTimerResolution",
                "Ulong",
                ('::std::uint_least32_t', 'bool', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwSetTimerResolution",
                "Ulong",
                ('::std::uint_least32_t', 'bool', '::std::uint_least32_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtCreateTimer",
                "Ulong",
                ('void **', '::std::uint_least32_t', 'object_attributes *', 'timer_type', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwCreateTimer",
                "Ulong",
                ('void **', '::std::uint_least32_t', 'object_attributes *', 'timer_type', ),
                nodiscard=True,
            ),
            Symbol(
                "NtSetTimer",
                "Ulong",
                ('void *', '::std::uint_least64_t const *', 'ptimer_apc_routine', 'void *', '::std::uint_least8_t', '::std::int_least32_t', '::std::uint_least8_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwSetTimer",
                "Ulong",
                ('void *', '::std::uint_least64_t const *', 'ptimer_apc_routine', 'void *', '::std::uint_least8_t', '::std::int_least32_t', '::std::uint_least8_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "NtCancelTimer",
                "Ulong",
                ('void *', '::std::uint_least8_t *', ),
                nodiscard=True,
            ),
            Symbol(
                "ZwCancelTimer",
                "Ulong",
                ('void *', '::std::uint_least8_t *', ),
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


def declaration(symbol: Symbol, x86_stack_bytes: int) -> str:
    attribute = "[[nodiscard]]\n" if symbol.nodiscard else ""
    convention = "PLTXT2HTM_DETAIL_STDCALL" if symbol.calling_convention == "stdcall" else "PLTXT2HTM_DETAIL_CDECL"
    suffix = (
        f" PLTXT2HTM_DETAIL_STDCALL_ASM_NAME({symbol.export_name}, {x86_stack_bytes})"
        if symbol.calling_convention == "stdcall"
        else f" PLTXT2HTM_DETAIL_CDECL_ASM_NAME({symbol.export_name})"
    )
    return (
        f"{attribute}PLTXT2HTM_DETAIL_DLLIMPORT {symbol.return_type} {convention} {symbol.export_name}"
        f"({format_parameters(symbol.parameters)}) noexcept{suffix};\n"
    )


def generate_declarations(library: Library, x86_stack_bytes: dict[str, int]) -> str:
    result = [GENERATED_NOTICE, "\n"]
    result.extend(
        declaration(symbol, x86_stack_bytes[symbol.export_name])
        for symbol in library.symbols
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
    names = [line.split()[0] for line in nm_result.stdout.decode("ascii").splitlines() if line.strip()]
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
    x86_stack_bytes: dict[str, int] = {}
    for symbol in library.symbols:
        pairs = undefined_symbols(symbol, library, target, clang, llvm_nm, cxx_include_dir)
        for local, exported in pairs:
            result.append(f'#pragma comment(linker, "/alternatename:{local}={exported}")\n')
        if target.startswith("i686-") and symbol.calling_convention == "stdcall":
            match = re.search(r"@(\d+)$", pairs[0][1])
            if match is None:
                raise RuntimeError(f"missing i686 stdcall decoration for {symbol.export_name}: {pairs[0][1]}")
            x86_stack_bytes[symbol.export_name] = int(match.group(1))
        elif target.startswith("i686-"):
            x86_stack_bytes[symbol.export_name] = 0
    return "".join(result), x86_stack_bytes


def generated_files(
    clang: str, llvm_nm: str, cxx_include_dir: pathlib.Path | None
) -> dict[pathlib.Path, str]:
    result: dict[pathlib.Path, str] = {}
    for library in LIBRARIES:
        directory = SYMBOL_ROOT / library.api
        i686_stack_bytes: dict[str, int] = {}
        for architecture, target in MSVC_TARGETS:
            aliases, target_stack_bytes = generate_linker_aliases(
                library, target, clang, llvm_nm, cxx_include_dir
            )
            result[directory / f"{library.name}_msvc_linker_{architecture}.inc"] = aliases
            i686_stack_bytes.update(target_stack_bytes)
        result[directory / f"{library.name}.inc"] = generate_declarations(library, i686_stack_bytes)
    return result


def resolve_tool(value: str, description: str) -> str:
    resolved = shutil.which(value)
    if resolved is None:
        raise RuntimeError(f"{description} was not found: {value}")
    return resolved


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="fail if generated files are stale")
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
    files = generated_files(clang, llvm_nm, arguments.cxx_include_dir)
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
