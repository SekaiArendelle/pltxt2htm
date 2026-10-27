#pragma once

// Capability concepts shared by the container test cases. They live in their own
// header so the case headers that are included into one doctest TU do not each
// redefine the same concept.

template<typename T>
concept can_call_empty = requires(T const& value) { value.empty(); };

template<typename T>
concept can_call_is_empty = requires(T const& value) { value.is_empty(); };

template<typename T>
concept can_subscript = requires(T& value) { value[0]; };
