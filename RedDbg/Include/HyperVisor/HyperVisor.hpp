#pragma once 
#include "CppSupport/CppSupport.hpp"
#include "TransparentMode/TransparentMode.hpp"
#include "HyperVisor/CommonTypes/CPUID.hpp"
#include "HyperVisor/CommonTypes/MSR.hpp"
#include "HyperVisor/CommonTypes/Segmentation.hpp"
#include "HyperVisor/CommonTypes/Registers.hpp"
#include "HyperVisor/CommonTypes/PTE.hpp"
#include "HyperVisor/CommonTypes/SVM.hpp"
#include "HyperVisor/CommonTypes/Exception.hpp"
#include "HyperVisor/CommonTypes/Interrupts.hpp"
#include "HyperVisor/CommonApi/Callable.hpp"
#include "HyperVisor/CommonApi/MemoryUtils.hpp"
#include "HyperVisor/CommonApi/PteUtils.hpp"
#include <HyperVisor/Npt/HookKernelProcessorData.hpp>
//#include "HyperVisor/Npt/NptHook.hpp"
//#include "HyperVisor/Npt/NptSandbox.hpp"

#include <fltKernel.h>
#include <intrin.h>

constexpr auto MAXHOOKS = 6000;

extern "C" typedef void(*_SvmVmmRun)(
	_In_ void* InitialVmmStackPointer);

extern "C" typedef SVM::PRIVATE_VM_DATA* (*_Interceptions)(
	_Inout_ SVM::PRIVATE_VM_DATA* Private);

enum IntelEnc : unsigned int { IEbx = 'uneG', IEdx = 'Ieni', IEcx = 'letn' };
enum AmdEnc : unsigned int { AEbx = 'htuA', AEdx = 'itne', AEcx = 'DMAc' };

enum class CpuVendor { CpuUnknown, CpuIntel, CpuAmd};

// Magic value, defined by hypervisor, triggers #VMEXIT and VMM shutdown:
constexpr unsigned int HyperSign = 0x1EE7C0DE;
constexpr unsigned int CPUID_VMM_SHUTDOWN = HyperSign;

enum Ncr3Types : unsigned char { Primary, Shadow, Sandbox, SandboxSingleStep };

typedef struct
{
	bool P;// Present
	bool RW;// Read/Write
	bool US;// User/Supervisor
	bool NX;// No Execute
} PTEAccess;

class HyperVisorSvm {
private:
	class NptHook
	{
	public:
		LIST_ENTRY	list_entry{};
		PMDL mdl{};						/*	mdl used for locking hooked pages	*/
		unsigned char* guest_physical_page = nullptr;	/*	guest physical address of the hooked page	*/
		void* hooked_page = nullptr;				/*	guest virtual address of the hooked page	*/
		PTE* hookless_npte = nullptr;		/*	nested PTE of page without hooks			*/
		PTE* hooked_npte = nullptr;		/*	nested PTE of page without hooks			*/
		PTE* hooked_pte = nullptr;		/*	guest PTE of the copy page with hooks		*/
		PTE* guest_pte = nullptr;			/*	guest PTE of the original page				*/
		int original_nx = 0;		/*	original NX value of the guest PTE			*/
		uintptr_t process_cr3 = 0;			/*	process where this hook resides in			*/
		void* address = nullptr;					/*	virtual address of hook			*/
		int ncr3_id = 0;				/*	NCR3 directory that this hook resides in	*/

		NptHook();
	};


	class NptHooks
	{
	private:
		int hook_count;
		NptHook* npt_hook_list;
	public:
		NptHooks();
		HyperVisorSvm::NptHook* ForEachHook(bool(HookCallback)(NptHook* hook_entry, void* data), void* callback_data);
		HyperVisorSvm::NptHook* SetNptHook(
			SVM::PRIVATE_VM_DATA* vcpu,
			void* address,
			unsigned char* patch,
			size_t patch_len,
			int ncr3_id);
		void UnsetHook(HyperVisorSvm::NptHook* hook_entry);
	};
private:
	inline static uintptr_t Ncr3Dirs[4];
	inline static SVM::NESTED_PAGING_TABLES Npts[4];
private:
	CpuVendor GetCpuVendor();
	bool VirtualizeProcessor();
	bool DevirtualizeProcessor(_Out_ PVOID& PrivateVmData);

	void BuildNestedPagingTables(
		_In_ const PHYSICAL_MEMORY_DESCRIPTOR* MemoryDescriptor,
		_Out_ SVM::NESTED_PAGING_TABLES* Npt,
		_Out_ PHOOK_DATA HookData,
		_In_ PTEAccess Flags);

	void FillVmcbSegmentAttributes(
		_Out_ SVM::VMCB_STATE_SAVE_AREA::VMCB_SEGMENT_ATTRIBUTE* Attribute,
		_In_ const SEGMENT_SELECTOR* Selector,
		_In_ const DESCRIPTOR_TABLE_REGISTER_LONG* Gdtr);
private:
	static inline volatile bool g_IsVirtualized;
	_SvmVmmRun SvmVmmRun; _Interceptions Interceptions;
public:
	PVOID PSvmVmmRun = nullptr, PInterceptions = nullptr;

	NptHooks objNptHooks;
public:
	bool DevirtualizeAllProcessors();
	bool VirtualizeAllProcessors();
	bool IsSvmSupported();
};