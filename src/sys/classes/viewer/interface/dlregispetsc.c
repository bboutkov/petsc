
#include <petscdraw.h>
#include <petscviewer.h>

extern PetscLogEvent PETSC_Barrier;

static PetscBool PetscSysPackageInitialized = PETSC_FALSE;
#undef __FUNCT__
#define __FUNCT__ "PetscSysFinalizePackage"
/*@C
  PetscSysFinalizePackage - This function destroys everything in the Petsc interface to Mathematica. It is
  called from PetscFinalize().

  Level: developer

.keywords: Petsc, destroy, package, mathematica
.seealso: PetscFinalize()
@*/
PetscErrorCode  PetscSysFinalizePackage(void)
{
  PetscFunctionBegin;
  PetscSysPackageInitialized = PETSC_FALSE;
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscSysInitializePackage"
/*@C
  PetscSysInitializePackage - This function initializes everything in the main Petsc package. It is called
  from PetscDLLibraryRegister() when using dynamic libraries, and on the call to PetscInitialize()
  when using static libraries.

  Input Parameter:
  path - The dynamic library path, or NULL

  Level: developer

.keywords: Petsc, initialize, package
.seealso: PetscInitialize()
@*/
PetscErrorCode  PetscSysInitializePackage(const char path[])
{
  char           logList[256];
  char           *className;
  PetscBool      opt;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  if (PetscSysPackageInitialized) PetscFunctionReturn(0);
  PetscSysPackageInitialized = PETSC_TRUE;
  /* Register Classes */
  ierr = PetscClassIdRegister("Object",&PETSC_OBJECT_CLASSID);CHKERRQ(ierr);
  ierr = PetscClassIdRegister("Container",&PETSC_CONTAINER_CLASSID);CHKERRQ(ierr);

  /* Register Events */
  ierr = PetscLogEventRegister("PetscBarrier", PETSC_SMALLEST_CLASSID,&PETSC_Barrier);CHKERRQ(ierr);
  /* Process info exclusions */
  ierr = PetscOptionsGetString(NULL, "-info_exclude", logList, 256, &opt);CHKERRQ(ierr);
  if (opt) {
    ierr = PetscStrstr(logList, "null", &className);CHKERRQ(ierr);
    if (className) {
      ierr = PetscInfoDeactivateClass(0);CHKERRQ(ierr);
    }
  }
  /* Process summary exclusions */
  ierr = PetscOptionsGetString(NULL, "-log_summary_exclude", logList, 256, &opt);CHKERRQ(ierr);
  if (opt) {
    ierr = PetscStrstr(logList, "null", &className);CHKERRQ(ierr);
    if (className) {
      ierr = PetscLogEventDeactivateClass(0);CHKERRQ(ierr);
    }
  }
  ierr = PetscRegisterFinalize(PetscSysFinalizePackage);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#if defined(PETSC_USE_DYNAMIC_LIBRARIES)

#if defined(PETSC_USE_SINGLE_LIBRARY)
PETSC_EXTERN_C PetscErrorCode PetscDLLibraryRegister_petscvec(const char[]);
PETSC_EXTERN_C PetscErrorCode PetscDLLibraryRegister_petscmat(const char[]);
PETSC_EXTERN_C PetscErrorCode PetscDLLibraryRegister_petscdm(const char[]);
PETSC_EXTERN_C PetscErrorCode PetscDLLibraryRegister_petscksp(const char[]);
PETSC_EXTERN_C PetscErrorCode PetscDLLibraryRegister_petscsnes(const char[]);
PETSC_EXTERN_C PetscErrorCode PetscDLLibraryRegister_petscts(const char[]);
#endif

#undef __FUNCT__
#if defined(PETSC_USE_SINGLE_LIBRARY)
#define __FUNCT__ "PetscDLLibraryRegister_petsc"
#else
#define __FUNCT__ "PetscDLLibraryRegister_petscsys"
#endif
/*
  PetscDLLibraryRegister - This function is called when the dynamic library it is in is opened.

  This one registers all the draw and PetscViewer objects.

  Input Parameter:
  path - library path
 */
#if defined(PETSC_USE_SINGLE_LIBRARY)
PETSC_EXTERN_C PetscErrorCode  PetscDLLibraryRegister_petsc(const char path[])
#else
PETSC_EXTERN_C PetscErrorCode  PetscDLLibraryRegister_petscsys(const char path[])
#endif
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  /*
      If we got here then PETSc was properly loaded
  */
  ierr = PetscSysInitializePackage(path);CHKERRQ(ierr);
  ierr = PetscDrawInitializePackage(path);CHKERRQ(ierr);
  ierr = PetscViewerInitializePackage(path);CHKERRQ(ierr);
  ierr = PetscRandomInitializePackage(path);CHKERRQ(ierr);

#if defined(PETSC_USE_SINGLE_LIBRARY)
  ierr = PetscDLLibraryRegister_petscvec(path);CHKERRQ(ierr);
  ierr = PetscDLLibraryRegister_petscmat(path);CHKERRQ(ierr);
  ierr = PetscDLLibraryRegister_petscdm(path);CHKERRQ(ierr);
  ierr = PetscDLLibraryRegister_petscksp(path);CHKERRQ(ierr);
  ierr = PetscDLLibraryRegister_petscsnes(path);CHKERRQ(ierr);
  ierr = PetscDLLibraryRegister_petscts(path);CHKERRQ(ierr);
#endif
  PetscFunctionReturn(0);
}
#endif


