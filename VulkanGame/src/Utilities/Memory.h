#pragma once
#include "Debugging/Log.h"

#ifdef DEBUG_CONFIG
inline uint32_t AllocatedRefCount = 0;
inline uint32_t AllocatedScopeRefCount = 0;

#define ALLOCATED_REF_COUNT AllocatedRefCount
#define ALLOCATED_SCOPEREF_COUNT AllocatedScopeRefCount

#define UNDEFINED_PTR 3722304989
#else
#define ALLOCATED_REF_COUNT
#define ALLOCATED_SCOPEREF_COUNT

#define UNDEFINED_PTR 0
#endif

template<typename T>
class ScopeRef
{
public:
	ScopeRef() noexcept = default;
	ScopeRef(T* _ptr) noexcept : ptr(_ptr)
	{
#ifdef DEBUG_CONFIG
		AllocatedScopeRefCount++;
#endif
	}

	~ScopeRef() noexcept { Release(); }

	ScopeRef(const ScopeRef&) = delete;

	inline operator bool() const noexcept { return ptr; }
	inline operator T* () const noexcept { return ptr; }
	inline T* operator->() const noexcept { return ptr; }
	inline T& operator*() const noexcept { return *ptr; }

	inline T* GetPtr() const noexcept { return ptr; }
	inline T& Get() const noexcept { return *ptr; }

	inline ScopeRef& operator=(const ScopeRef&) = delete;

	inline ScopeRef& operator=(ScopeRef&& other) noexcept
	{
		if (this != &other)
		{
			delete ptr;
			ptr = other.ptr;
			other.ptr = nullptr;
		}

		return *this;
	}

	inline void Release() noexcept
	{
		delete ptr;
		ptr = nullptr;

#ifdef DEBUG_CONFIG
		AllocatedScopeRefCount--;
#endif
	}
private:
	T* ptr = nullptr;
};

template<typename T>
class Ref
{
public:
	Ref(T* _ptr = nullptr) : ptr(_ptr), refs(new uint32_t(1))
	{
#ifdef DEBUG_CONFIG
		AllocatedRefCount++;
#endif
	}

	Ref(T* _ptr, uint32_t* _refs) noexcept : ptr(_ptr), refs(_refs) { ++(*refs); }
	Ref(const Ref& other) noexcept : ptr(other.ptr), refs(other.refs) { ++(*refs); }

	Ref(Ref&& other) noexcept : ptr(other.ptr), refs(other.refs)
	{
		other.ptr = nullptr;
		other.refs = nullptr;
	}

	~Ref() noexcept { Release(); }

	inline operator bool() const noexcept { return ptr; }
	inline operator T* () const noexcept { return ptr; }
	inline T* operator->() const noexcept { return ptr; }
	inline T& operator*() const noexcept { return *ptr; }

	inline T* GetPtr() const noexcept { return ptr; }
	inline T& Get() const noexcept { return *ptr; }

	template<typename T2>
	inline Ref<T2> As() const noexcept { return Ref<T2>(ptr, refs); }

	inline const uint32_t& GetRefCount() const noexcept { return *refs; }

	inline bool operator==(const Ref& other) const noexcept { return ptr == other.ptr; }

	inline Ref& operator=(const Ref& other) noexcept
	{
		if (this != &other)
		{
			Release();
			ptr = other.ptr;
			refs = other.refs;
			++(*refs);
		}

		return *this;
	}

	inline Ref& operator=(Ref&& other) noexcept
	{
		if (this != &other)
		{
			Release();
			ptr = other.ptr;
			refs = other.refs;
			other.ptr = nullptr;
			other.refs = nullptr;
		}

		return *this;
	}

	inline void Release() noexcept
	{
		if (refs && --(*refs) == 0)
		{
			delete ptr;
			delete refs;
			ptr = nullptr;
			refs = nullptr;

#ifdef DEBUG_CONFIG
			AllocatedRefCount--;
#endif
		}
	}
private:
	T* ptr = nullptr;
	uint32_t* refs;
};

template<typename T>
class WeakRef
{
public:
	WeakRef() noexcept = default;
	WeakRef(const Ref<T>& ref) noexcept : ptr(ref.GetPtr()), refs((uint32_t*)&ref.GetRefCount()) {}
	~WeakRef() noexcept = default;

	inline operator bool() const noexcept { return IsValid(); }
	inline operator T* () const noexcept { return ptr; }
	inline T* operator->() const noexcept { return ptr; }
	inline T& operator*() const noexcept { return *ptr; }

	inline T* GetPtr() const noexcept { return ptr; }
	inline T& Get() const noexcept { return *ptr; }

	inline const bool IsValid() const noexcept { return refs && *refs != UNDEFINED_PTR; }
	inline Ref<T> Lock() const noexcept { return IsValid() ? Ref<T>(ptr, refs) : Ref<T>(nullptr); }
private:
	T* ptr = nullptr;
	uint32_t* refs = 0;
};