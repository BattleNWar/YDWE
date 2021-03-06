#include "storm.h"
#include <base/hook/fp_call.h>

namespace base { namespace warcraft3 {

	storm::storm() 
		: mod_(::LoadLibraryW(L"Storm.dll"))
		, valid_(load())
	{ }

	storm::~storm()
	{
		if (mod_)
		{
			::FreeLibrary(mod_);
			mod_ = NULL;
		}
	}

	bool storm::load()
	{
		if (!mod_) return false;
		fn_sfile_load_file_   = get_proc(281); if (!(fn_sfile_load_file_))   return false; 
		fn_sfile_unload_file_ = get_proc(280); if (!(fn_sfile_unload_file_)) return false; 
		fn_sfile_exists_      = get_proc(288); if (!(fn_sfile_exists_))      return false; 

		return true;
	}

	bool storm::valid() const
	{
		return valid_;
	}

	bool storm::load_file(const char* file_name, const void** buffer_ptr, size_t* size_ptr)
	{
		return load_file(NULL, file_name, buffer_ptr, size_ptr, 0, 1, NULL);
	}

	bool storm::load_file(HANDLE archive_handle, const char* file_name, const void** buffer_ptr, size_t* size_ptr, size_t reserve_size, uint32_t search_scope, OVERLAPPED* overlapped_ptr)
	{
		if (!valid()) return false;
		return !!std_call<BOOL>(fn_sfile_load_file_, archive_handle, file_name, buffer_ptr, size_ptr, reserve_size, search_scope, overlapped_ptr);
	}

	bool storm::unload_file(const void* buffer)
	{
		if (!valid()) return false;
		return !!std_call<BOOL>(fn_sfile_unload_file_, buffer);
	}

	bool storm::has_file(const char* file_name)
	{
		if (!valid()) return false;
		return !!std_call<BOOL>(fn_sfile_exists_, file_name);
	}

	uintptr_t storm::get_proc(uint32_t ord)
	{
		return (uintptr_t)::GetProcAddress(mod_, (const char*)ord);
	}
}}
