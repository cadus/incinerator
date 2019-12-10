#pragma once

#define NOT_COPYABLE(TypeName) \
TypeName (TypeName const&) = delete; \
TypeName & operator = (TypeName const&) = delete;

#define NOT_MOVEABLE(TypeName) \
TypeName (TypeName &&) = delete; \
TypeName & operator = (TypeName &&) = delete;
