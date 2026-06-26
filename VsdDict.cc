// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME VsdDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "VsdBase.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *VsdBase_Dictionary();
   static void VsdBase_TClassManip(TClass*);
   static void *new_VsdBase(void *p = nullptr);
   static void *newArray_VsdBase(Long_t size, void *p);
   static void delete_VsdBase(void *p);
   static void deleteArray_VsdBase(void *p);
   static void destruct_VsdBase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::VsdBase*)
   {
      ::VsdBase *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::VsdBase));
      static ::ROOT::TGenericClassInfo 
         instance("VsdBase", "VsdBase.h", 14,
                  typeid(::VsdBase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &VsdBase_Dictionary, isa_proxy, 4,
                  sizeof(::VsdBase), alignof(::VsdBase) );
      instance.SetNew(&new_VsdBase);
      instance.SetNewArray(&newArray_VsdBase);
      instance.SetDelete(&delete_VsdBase);
      instance.SetDeleteArray(&deleteArray_VsdBase);
      instance.SetDestructor(&destruct_VsdBase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::VsdBase*)
   {
      return GenerateInitInstanceLocal(static_cast<::VsdBase*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::VsdBase*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *VsdBase_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::VsdBase*>(nullptr))->GetClass();
      VsdBase_TClassManip(theClass);
   return theClass;
   }

   static void VsdBase_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *VsdVertex_Dictionary();
   static void VsdVertex_TClassManip(TClass*);
   static void *new_VsdVertex(void *p = nullptr);
   static void *newArray_VsdVertex(Long_t size, void *p);
   static void delete_VsdVertex(void *p);
   static void deleteArray_VsdVertex(void *p);
   static void destruct_VsdVertex(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::VsdVertex*)
   {
      ::VsdVertex *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::VsdVertex));
      static ::ROOT::TGenericClassInfo 
         instance("VsdVertex", "VsdBase.h", 22,
                  typeid(::VsdVertex), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &VsdVertex_Dictionary, isa_proxy, 4,
                  sizeof(::VsdVertex), alignof(::VsdVertex) );
      instance.SetNew(&new_VsdVertex);
      instance.SetNewArray(&newArray_VsdVertex);
      instance.SetDelete(&delete_VsdVertex);
      instance.SetDeleteArray(&deleteArray_VsdVertex);
      instance.SetDestructor(&destruct_VsdVertex);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::VsdVertex*)
   {
      return GenerateInitInstanceLocal(static_cast<::VsdVertex*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::VsdVertex*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *VsdVertex_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::VsdVertex*>(nullptr))->GetClass();
      VsdVertex_TClassManip(theClass);
   return theClass;
   }

   static void VsdVertex_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *VsdCandidate_Dictionary();
   static void VsdCandidate_TClassManip(TClass*);
   static void *new_VsdCandidate(void *p = nullptr);
   static void *newArray_VsdCandidate(Long_t size, void *p);
   static void delete_VsdCandidate(void *p);
   static void deleteArray_VsdCandidate(void *p);
   static void destruct_VsdCandidate(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::VsdCandidate*)
   {
      ::VsdCandidate *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::VsdCandidate));
      static ::ROOT::TGenericClassInfo 
         instance("VsdCandidate", "VsdBase.h", 58,
                  typeid(::VsdCandidate), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &VsdCandidate_Dictionary, isa_proxy, 4,
                  sizeof(::VsdCandidate), alignof(::VsdCandidate) );
      instance.SetNew(&new_VsdCandidate);
      instance.SetNewArray(&newArray_VsdCandidate);
      instance.SetDelete(&delete_VsdCandidate);
      instance.SetDeleteArray(&deleteArray_VsdCandidate);
      instance.SetDestructor(&destruct_VsdCandidate);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::VsdCandidate*)
   {
      return GenerateInitInstanceLocal(static_cast<::VsdCandidate*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::VsdCandidate*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *VsdCandidate_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::VsdCandidate*>(nullptr))->GetClass();
      VsdCandidate_TClassManip(theClass);
   return theClass;
   }

   static void VsdCandidate_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *VsdCaloTower_Dictionary();
   static void VsdCaloTower_TClassManip(TClass*);
   static void *new_VsdCaloTower(void *p = nullptr);
   static void *newArray_VsdCaloTower(Long_t size, void *p);
   static void delete_VsdCaloTower(void *p);
   static void deleteArray_VsdCaloTower(void *p);
   static void destruct_VsdCaloTower(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::VsdCaloTower*)
   {
      ::VsdCaloTower *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::VsdCaloTower));
      static ::ROOT::TGenericClassInfo 
         instance("VsdCaloTower", "VsdBase.h", 96,
                  typeid(::VsdCaloTower), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &VsdCaloTower_Dictionary, isa_proxy, 4,
                  sizeof(::VsdCaloTower), alignof(::VsdCaloTower) );
      instance.SetNew(&new_VsdCaloTower);
      instance.SetNewArray(&newArray_VsdCaloTower);
      instance.SetDelete(&delete_VsdCaloTower);
      instance.SetDeleteArray(&deleteArray_VsdCaloTower);
      instance.SetDestructor(&destruct_VsdCaloTower);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::VsdCaloTower*)
   {
      return GenerateInitInstanceLocal(static_cast<::VsdCaloTower*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::VsdCaloTower*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *VsdCaloTower_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::VsdCaloTower*>(nullptr))->GetClass();
      VsdCaloTower_TClassManip(theClass);
   return theClass;
   }

   static void VsdCaloTower_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *VsdJet_Dictionary();
   static void VsdJet_TClassManip(TClass*);
   static void *new_VsdJet(void *p = nullptr);
   static void *newArray_VsdJet(Long_t size, void *p);
   static void delete_VsdJet(void *p);
   static void deleteArray_VsdJet(void *p);
   static void destruct_VsdJet(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::VsdJet*)
   {
      ::VsdJet *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::VsdJet));
      static ::ROOT::TGenericClassInfo 
         instance("VsdJet", "VsdBase.h", 105,
                  typeid(::VsdJet), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &VsdJet_Dictionary, isa_proxy, 4,
                  sizeof(::VsdJet), alignof(::VsdJet) );
      instance.SetNew(&new_VsdJet);
      instance.SetNewArray(&newArray_VsdJet);
      instance.SetDelete(&delete_VsdJet);
      instance.SetDeleteArray(&deleteArray_VsdJet);
      instance.SetDestructor(&destruct_VsdJet);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::VsdJet*)
   {
      return GenerateInitInstanceLocal(static_cast<::VsdJet*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::VsdJet*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *VsdJet_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::VsdJet*>(nullptr))->GetClass();
      VsdJet_TClassManip(theClass);
   return theClass;
   }

   static void VsdJet_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *VsdMuon_Dictionary();
   static void VsdMuon_TClassManip(TClass*);
   static void *new_VsdMuon(void *p = nullptr);
   static void *newArray_VsdMuon(Long_t size, void *p);
   static void delete_VsdMuon(void *p);
   static void deleteArray_VsdMuon(void *p);
   static void destruct_VsdMuon(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::VsdMuon*)
   {
      ::VsdMuon *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::VsdMuon));
      static ::ROOT::TGenericClassInfo 
         instance("VsdMuon", "VsdBase.h", 127,
                  typeid(::VsdMuon), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &VsdMuon_Dictionary, isa_proxy, 4,
                  sizeof(::VsdMuon), alignof(::VsdMuon) );
      instance.SetNew(&new_VsdMuon);
      instance.SetNewArray(&newArray_VsdMuon);
      instance.SetDelete(&delete_VsdMuon);
      instance.SetDeleteArray(&deleteArray_VsdMuon);
      instance.SetDestructor(&destruct_VsdMuon);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::VsdMuon*)
   {
      return GenerateInitInstanceLocal(static_cast<::VsdMuon*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::VsdMuon*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *VsdMuon_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::VsdMuon*>(nullptr))->GetClass();
      VsdMuon_TClassManip(theClass);
   return theClass;
   }

   static void VsdMuon_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *VsdMET_Dictionary();
   static void VsdMET_TClassManip(TClass*);
   static void *new_VsdMET(void *p = nullptr);
   static void *newArray_VsdMET(Long_t size, void *p);
   static void delete_VsdMET(void *p);
   static void deleteArray_VsdMET(void *p);
   static void destruct_VsdMET(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::VsdMET*)
   {
      ::VsdMET *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::VsdMET));
      static ::ROOT::TGenericClassInfo 
         instance("VsdMET", "VsdBase.h", 141,
                  typeid(::VsdMET), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &VsdMET_Dictionary, isa_proxy, 4,
                  sizeof(::VsdMET), alignof(::VsdMET) );
      instance.SetNew(&new_VsdMET);
      instance.SetNewArray(&newArray_VsdMET);
      instance.SetDelete(&delete_VsdMET);
      instance.SetDeleteArray(&deleteArray_VsdMET);
      instance.SetDestructor(&destruct_VsdMET);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::VsdMET*)
   {
      return GenerateInitInstanceLocal(static_cast<::VsdMET*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::VsdMET*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *VsdMET_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::VsdMET*>(nullptr))->GetClass();
      VsdMET_TClassManip(theClass);
   return theClass;
   }

   static void VsdMET_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *VsdEventInfo_Dictionary();
   static void VsdEventInfo_TClassManip(TClass*);
   static void *new_VsdEventInfo(void *p = nullptr);
   static void *newArray_VsdEventInfo(Long_t size, void *p);
   static void delete_VsdEventInfo(void *p);
   static void deleteArray_VsdEventInfo(void *p);
   static void destruct_VsdEventInfo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::VsdEventInfo*)
   {
      ::VsdEventInfo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::VsdEventInfo));
      static ::ROOT::TGenericClassInfo 
         instance("VsdEventInfo", "VsdBase.h", 218,
                  typeid(::VsdEventInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &VsdEventInfo_Dictionary, isa_proxy, 4,
                  sizeof(::VsdEventInfo), alignof(::VsdEventInfo) );
      instance.SetNew(&new_VsdEventInfo);
      instance.SetNewArray(&newArray_VsdEventInfo);
      instance.SetDelete(&delete_VsdEventInfo);
      instance.SetDeleteArray(&deleteArray_VsdEventInfo);
      instance.SetDestructor(&destruct_VsdEventInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::VsdEventInfo*)
   {
      return GenerateInitInstanceLocal(static_cast<::VsdEventInfo*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::VsdEventInfo*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *VsdEventInfo_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::VsdEventInfo*>(nullptr))->GetClass();
      VsdEventInfo_TClassManip(theClass);
   return theClass;
   }

   static void VsdEventInfo_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_VsdBase(void *p) {
      return  p ? new(p) ::VsdBase : new ::VsdBase;
   }
   static void *newArray_VsdBase(Long_t nElements, void *p) {
      return p ? new(p) ::VsdBase[nElements] : new ::VsdBase[nElements];
   }
   // Wrapper around operator delete
   static void delete_VsdBase(void *p) {
      delete (static_cast<::VsdBase*>(p));
   }
   static void deleteArray_VsdBase(void *p) {
      delete [] (static_cast<::VsdBase*>(p));
   }
   static void destruct_VsdBase(void *p) {
      typedef ::VsdBase current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::VsdBase

namespace ROOT {
   // Wrappers around operator new
   static void *new_VsdVertex(void *p) {
      return  p ? new(p) ::VsdVertex : new ::VsdVertex;
   }
   static void *newArray_VsdVertex(Long_t nElements, void *p) {
      return p ? new(p) ::VsdVertex[nElements] : new ::VsdVertex[nElements];
   }
   // Wrapper around operator delete
   static void delete_VsdVertex(void *p) {
      delete (static_cast<::VsdVertex*>(p));
   }
   static void deleteArray_VsdVertex(void *p) {
      delete [] (static_cast<::VsdVertex*>(p));
   }
   static void destruct_VsdVertex(void *p) {
      typedef ::VsdVertex current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::VsdVertex

namespace ROOT {
   // Wrappers around operator new
   static void *new_VsdCandidate(void *p) {
      return  p ? new(p) ::VsdCandidate : new ::VsdCandidate;
   }
   static void *newArray_VsdCandidate(Long_t nElements, void *p) {
      return p ? new(p) ::VsdCandidate[nElements] : new ::VsdCandidate[nElements];
   }
   // Wrapper around operator delete
   static void delete_VsdCandidate(void *p) {
      delete (static_cast<::VsdCandidate*>(p));
   }
   static void deleteArray_VsdCandidate(void *p) {
      delete [] (static_cast<::VsdCandidate*>(p));
   }
   static void destruct_VsdCandidate(void *p) {
      typedef ::VsdCandidate current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::VsdCandidate

namespace ROOT {
   // Wrappers around operator new
   static void *new_VsdCaloTower(void *p) {
      return  p ? new(p) ::VsdCaloTower : new ::VsdCaloTower;
   }
   static void *newArray_VsdCaloTower(Long_t nElements, void *p) {
      return p ? new(p) ::VsdCaloTower[nElements] : new ::VsdCaloTower[nElements];
   }
   // Wrapper around operator delete
   static void delete_VsdCaloTower(void *p) {
      delete (static_cast<::VsdCaloTower*>(p));
   }
   static void deleteArray_VsdCaloTower(void *p) {
      delete [] (static_cast<::VsdCaloTower*>(p));
   }
   static void destruct_VsdCaloTower(void *p) {
      typedef ::VsdCaloTower current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::VsdCaloTower

namespace ROOT {
   // Wrappers around operator new
   static void *new_VsdJet(void *p) {
      return  p ? new(p) ::VsdJet : new ::VsdJet;
   }
   static void *newArray_VsdJet(Long_t nElements, void *p) {
      return p ? new(p) ::VsdJet[nElements] : new ::VsdJet[nElements];
   }
   // Wrapper around operator delete
   static void delete_VsdJet(void *p) {
      delete (static_cast<::VsdJet*>(p));
   }
   static void deleteArray_VsdJet(void *p) {
      delete [] (static_cast<::VsdJet*>(p));
   }
   static void destruct_VsdJet(void *p) {
      typedef ::VsdJet current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::VsdJet

namespace ROOT {
   // Wrappers around operator new
   static void *new_VsdMuon(void *p) {
      return  p ? new(p) ::VsdMuon : new ::VsdMuon;
   }
   static void *newArray_VsdMuon(Long_t nElements, void *p) {
      return p ? new(p) ::VsdMuon[nElements] : new ::VsdMuon[nElements];
   }
   // Wrapper around operator delete
   static void delete_VsdMuon(void *p) {
      delete (static_cast<::VsdMuon*>(p));
   }
   static void deleteArray_VsdMuon(void *p) {
      delete [] (static_cast<::VsdMuon*>(p));
   }
   static void destruct_VsdMuon(void *p) {
      typedef ::VsdMuon current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::VsdMuon

namespace ROOT {
   // Wrappers around operator new
   static void *new_VsdMET(void *p) {
      return  p ? new(p) ::VsdMET : new ::VsdMET;
   }
   static void *newArray_VsdMET(Long_t nElements, void *p) {
      return p ? new(p) ::VsdMET[nElements] : new ::VsdMET[nElements];
   }
   // Wrapper around operator delete
   static void delete_VsdMET(void *p) {
      delete (static_cast<::VsdMET*>(p));
   }
   static void deleteArray_VsdMET(void *p) {
      delete [] (static_cast<::VsdMET*>(p));
   }
   static void destruct_VsdMET(void *p) {
      typedef ::VsdMET current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::VsdMET

namespace ROOT {
   // Wrappers around operator new
   static void *new_VsdEventInfo(void *p) {
      return  p ? new(p) ::VsdEventInfo : new ::VsdEventInfo;
   }
   static void *newArray_VsdEventInfo(Long_t nElements, void *p) {
      return p ? new(p) ::VsdEventInfo[nElements] : new ::VsdEventInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_VsdEventInfo(void *p) {
      delete (static_cast<::VsdEventInfo*>(p));
   }
   static void deleteArray_VsdEventInfo(void *p) {
      delete [] (static_cast<::VsdEventInfo*>(p));
   }
   static void destruct_VsdEventInfo(void *p) {
      typedef ::VsdEventInfo current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::VsdEventInfo

namespace ROOT {
   static TClass *vectorlEVsdVertexgR_Dictionary();
   static void vectorlEVsdVertexgR_TClassManip(TClass*);
   static void *new_vectorlEVsdVertexgR(void *p = nullptr);
   static void *newArray_vectorlEVsdVertexgR(Long_t size, void *p);
   static void delete_vectorlEVsdVertexgR(void *p);
   static void deleteArray_vectorlEVsdVertexgR(void *p);
   static void destruct_vectorlEVsdVertexgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<VsdVertex>*)
   {
      vector<VsdVertex> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<VsdVertex>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<VsdVertex>", -2, "vector", 389,
                  typeid(vector<VsdVertex>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEVsdVertexgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<VsdVertex>), alignof(vector<VsdVertex>) );
      instance.SetNew(&new_vectorlEVsdVertexgR);
      instance.SetNewArray(&newArray_vectorlEVsdVertexgR);
      instance.SetDelete(&delete_vectorlEVsdVertexgR);
      instance.SetDeleteArray(&deleteArray_vectorlEVsdVertexgR);
      instance.SetDestructor(&destruct_vectorlEVsdVertexgR);
      static_assert(alignof(vector<VsdVertex>::value_type) <= 4096,
          "Class with alignment strictly greater than 4096 are currently not supported in CollectionProxy. "
          "Please report this case to the developers");
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<VsdVertex> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<VsdVertex>","std::vector<VsdVertex, std::allocator<VsdVertex> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<VsdVertex>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEVsdVertexgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<VsdVertex>*>(nullptr))->GetClass();
      vectorlEVsdVertexgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEVsdVertexgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEVsdVertexgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<VsdVertex> : new vector<VsdVertex>;
   }
   static void *newArray_vectorlEVsdVertexgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<VsdVertex>[nElements] : new vector<VsdVertex>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEVsdVertexgR(void *p) {
      delete (static_cast<vector<VsdVertex>*>(p));
   }
   static void deleteArray_vectorlEVsdVertexgR(void *p) {
      delete [] (static_cast<vector<VsdVertex>*>(p));
   }
   static void destruct_vectorlEVsdVertexgR(void *p) {
      typedef vector<VsdVertex> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<VsdVertex>

namespace ROOT {
   static TClass *vectorlEVsdMuongR_Dictionary();
   static void vectorlEVsdMuongR_TClassManip(TClass*);
   static void *new_vectorlEVsdMuongR(void *p = nullptr);
   static void *newArray_vectorlEVsdMuongR(Long_t size, void *p);
   static void delete_vectorlEVsdMuongR(void *p);
   static void deleteArray_vectorlEVsdMuongR(void *p);
   static void destruct_vectorlEVsdMuongR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<VsdMuon>*)
   {
      vector<VsdMuon> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<VsdMuon>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<VsdMuon>", -2, "vector", 389,
                  typeid(vector<VsdMuon>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEVsdMuongR_Dictionary, isa_proxy, 4,
                  sizeof(vector<VsdMuon>), alignof(vector<VsdMuon>) );
      instance.SetNew(&new_vectorlEVsdMuongR);
      instance.SetNewArray(&newArray_vectorlEVsdMuongR);
      instance.SetDelete(&delete_vectorlEVsdMuongR);
      instance.SetDeleteArray(&deleteArray_vectorlEVsdMuongR);
      instance.SetDestructor(&destruct_vectorlEVsdMuongR);
      static_assert(alignof(vector<VsdMuon>::value_type) <= 4096,
          "Class with alignment strictly greater than 4096 are currently not supported in CollectionProxy. "
          "Please report this case to the developers");
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<VsdMuon> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<VsdMuon>","std::vector<VsdMuon, std::allocator<VsdMuon> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<VsdMuon>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEVsdMuongR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<VsdMuon>*>(nullptr))->GetClass();
      vectorlEVsdMuongR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEVsdMuongR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEVsdMuongR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<VsdMuon> : new vector<VsdMuon>;
   }
   static void *newArray_vectorlEVsdMuongR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<VsdMuon>[nElements] : new vector<VsdMuon>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEVsdMuongR(void *p) {
      delete (static_cast<vector<VsdMuon>*>(p));
   }
   static void deleteArray_vectorlEVsdMuongR(void *p) {
      delete [] (static_cast<vector<VsdMuon>*>(p));
   }
   static void destruct_vectorlEVsdMuongR(void *p) {
      typedef vector<VsdMuon> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<VsdMuon>

namespace ROOT {
   static TClass *vectorlEVsdMETgR_Dictionary();
   static void vectorlEVsdMETgR_TClassManip(TClass*);
   static void *new_vectorlEVsdMETgR(void *p = nullptr);
   static void *newArray_vectorlEVsdMETgR(Long_t size, void *p);
   static void delete_vectorlEVsdMETgR(void *p);
   static void deleteArray_vectorlEVsdMETgR(void *p);
   static void destruct_vectorlEVsdMETgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<VsdMET>*)
   {
      vector<VsdMET> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<VsdMET>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<VsdMET>", -2, "vector", 389,
                  typeid(vector<VsdMET>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEVsdMETgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<VsdMET>), alignof(vector<VsdMET>) );
      instance.SetNew(&new_vectorlEVsdMETgR);
      instance.SetNewArray(&newArray_vectorlEVsdMETgR);
      instance.SetDelete(&delete_vectorlEVsdMETgR);
      instance.SetDeleteArray(&deleteArray_vectorlEVsdMETgR);
      instance.SetDestructor(&destruct_vectorlEVsdMETgR);
      static_assert(alignof(vector<VsdMET>::value_type) <= 4096,
          "Class with alignment strictly greater than 4096 are currently not supported in CollectionProxy. "
          "Please report this case to the developers");
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<VsdMET> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<VsdMET>","std::vector<VsdMET, std::allocator<VsdMET> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<VsdMET>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEVsdMETgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<VsdMET>*>(nullptr))->GetClass();
      vectorlEVsdMETgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEVsdMETgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEVsdMETgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<VsdMET> : new vector<VsdMET>;
   }
   static void *newArray_vectorlEVsdMETgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<VsdMET>[nElements] : new vector<VsdMET>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEVsdMETgR(void *p) {
      delete (static_cast<vector<VsdMET>*>(p));
   }
   static void deleteArray_vectorlEVsdMETgR(void *p) {
      delete [] (static_cast<vector<VsdMET>*>(p));
   }
   static void destruct_vectorlEVsdMETgR(void *p) {
      typedef vector<VsdMET> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<VsdMET>

namespace ROOT {
   static TClass *vectorlEVsdJetgR_Dictionary();
   static void vectorlEVsdJetgR_TClassManip(TClass*);
   static void *new_vectorlEVsdJetgR(void *p = nullptr);
   static void *newArray_vectorlEVsdJetgR(Long_t size, void *p);
   static void delete_vectorlEVsdJetgR(void *p);
   static void deleteArray_vectorlEVsdJetgR(void *p);
   static void destruct_vectorlEVsdJetgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<VsdJet>*)
   {
      vector<VsdJet> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<VsdJet>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<VsdJet>", -2, "vector", 389,
                  typeid(vector<VsdJet>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEVsdJetgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<VsdJet>), alignof(vector<VsdJet>) );
      instance.SetNew(&new_vectorlEVsdJetgR);
      instance.SetNewArray(&newArray_vectorlEVsdJetgR);
      instance.SetDelete(&delete_vectorlEVsdJetgR);
      instance.SetDeleteArray(&deleteArray_vectorlEVsdJetgR);
      instance.SetDestructor(&destruct_vectorlEVsdJetgR);
      static_assert(alignof(vector<VsdJet>::value_type) <= 4096,
          "Class with alignment strictly greater than 4096 are currently not supported in CollectionProxy. "
          "Please report this case to the developers");
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<VsdJet> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<VsdJet>","std::vector<VsdJet, std::allocator<VsdJet> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<VsdJet>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEVsdJetgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<VsdJet>*>(nullptr))->GetClass();
      vectorlEVsdJetgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEVsdJetgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEVsdJetgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<VsdJet> : new vector<VsdJet>;
   }
   static void *newArray_vectorlEVsdJetgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<VsdJet>[nElements] : new vector<VsdJet>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEVsdJetgR(void *p) {
      delete (static_cast<vector<VsdJet>*>(p));
   }
   static void deleteArray_vectorlEVsdJetgR(void *p) {
      delete [] (static_cast<vector<VsdJet>*>(p));
   }
   static void destruct_vectorlEVsdJetgR(void *p) {
      typedef vector<VsdJet> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<VsdJet>

namespace ROOT {
   static TClass *vectorlEVsdEventInfogR_Dictionary();
   static void vectorlEVsdEventInfogR_TClassManip(TClass*);
   static void *new_vectorlEVsdEventInfogR(void *p = nullptr);
   static void *newArray_vectorlEVsdEventInfogR(Long_t size, void *p);
   static void delete_vectorlEVsdEventInfogR(void *p);
   static void deleteArray_vectorlEVsdEventInfogR(void *p);
   static void destruct_vectorlEVsdEventInfogR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<VsdEventInfo>*)
   {
      vector<VsdEventInfo> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<VsdEventInfo>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<VsdEventInfo>", -2, "vector", 389,
                  typeid(vector<VsdEventInfo>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEVsdEventInfogR_Dictionary, isa_proxy, 4,
                  sizeof(vector<VsdEventInfo>), alignof(vector<VsdEventInfo>) );
      instance.SetNew(&new_vectorlEVsdEventInfogR);
      instance.SetNewArray(&newArray_vectorlEVsdEventInfogR);
      instance.SetDelete(&delete_vectorlEVsdEventInfogR);
      instance.SetDeleteArray(&deleteArray_vectorlEVsdEventInfogR);
      instance.SetDestructor(&destruct_vectorlEVsdEventInfogR);
      static_assert(alignof(vector<VsdEventInfo>::value_type) <= 4096,
          "Class with alignment strictly greater than 4096 are currently not supported in CollectionProxy. "
          "Please report this case to the developers");
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<VsdEventInfo> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<VsdEventInfo>","std::vector<VsdEventInfo, std::allocator<VsdEventInfo> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<VsdEventInfo>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEVsdEventInfogR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<VsdEventInfo>*>(nullptr))->GetClass();
      vectorlEVsdEventInfogR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEVsdEventInfogR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEVsdEventInfogR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<VsdEventInfo> : new vector<VsdEventInfo>;
   }
   static void *newArray_vectorlEVsdEventInfogR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<VsdEventInfo>[nElements] : new vector<VsdEventInfo>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEVsdEventInfogR(void *p) {
      delete (static_cast<vector<VsdEventInfo>*>(p));
   }
   static void deleteArray_vectorlEVsdEventInfogR(void *p) {
      delete [] (static_cast<vector<VsdEventInfo>*>(p));
   }
   static void destruct_vectorlEVsdEventInfogR(void *p) {
      typedef vector<VsdEventInfo> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<VsdEventInfo>

namespace ROOT {
   static TClass *vectorlEVsdCandidategR_Dictionary();
   static void vectorlEVsdCandidategR_TClassManip(TClass*);
   static void *new_vectorlEVsdCandidategR(void *p = nullptr);
   static void *newArray_vectorlEVsdCandidategR(Long_t size, void *p);
   static void delete_vectorlEVsdCandidategR(void *p);
   static void deleteArray_vectorlEVsdCandidategR(void *p);
   static void destruct_vectorlEVsdCandidategR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<VsdCandidate>*)
   {
      vector<VsdCandidate> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<VsdCandidate>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<VsdCandidate>", -2, "vector", 389,
                  typeid(vector<VsdCandidate>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEVsdCandidategR_Dictionary, isa_proxy, 4,
                  sizeof(vector<VsdCandidate>), alignof(vector<VsdCandidate>) );
      instance.SetNew(&new_vectorlEVsdCandidategR);
      instance.SetNewArray(&newArray_vectorlEVsdCandidategR);
      instance.SetDelete(&delete_vectorlEVsdCandidategR);
      instance.SetDeleteArray(&deleteArray_vectorlEVsdCandidategR);
      instance.SetDestructor(&destruct_vectorlEVsdCandidategR);
      static_assert(alignof(vector<VsdCandidate>::value_type) <= 4096,
          "Class with alignment strictly greater than 4096 are currently not supported in CollectionProxy. "
          "Please report this case to the developers");
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<VsdCandidate> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<VsdCandidate>","std::vector<VsdCandidate, std::allocator<VsdCandidate> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<VsdCandidate>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEVsdCandidategR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<VsdCandidate>*>(nullptr))->GetClass();
      vectorlEVsdCandidategR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEVsdCandidategR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEVsdCandidategR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<VsdCandidate> : new vector<VsdCandidate>;
   }
   static void *newArray_vectorlEVsdCandidategR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<VsdCandidate>[nElements] : new vector<VsdCandidate>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEVsdCandidategR(void *p) {
      delete (static_cast<vector<VsdCandidate>*>(p));
   }
   static void deleteArray_vectorlEVsdCandidategR(void *p) {
      delete [] (static_cast<vector<VsdCandidate>*>(p));
   }
   static void destruct_vectorlEVsdCandidategR(void *p) {
      typedef vector<VsdCandidate> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<VsdCandidate>

namespace ROOT {
   static TClass *vectorlEVsdCaloTowergR_Dictionary();
   static void vectorlEVsdCaloTowergR_TClassManip(TClass*);
   static void *new_vectorlEVsdCaloTowergR(void *p = nullptr);
   static void *newArray_vectorlEVsdCaloTowergR(Long_t size, void *p);
   static void delete_vectorlEVsdCaloTowergR(void *p);
   static void deleteArray_vectorlEVsdCaloTowergR(void *p);
   static void destruct_vectorlEVsdCaloTowergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<VsdCaloTower>*)
   {
      vector<VsdCaloTower> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<VsdCaloTower>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<VsdCaloTower>", -2, "vector", 389,
                  typeid(vector<VsdCaloTower>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEVsdCaloTowergR_Dictionary, isa_proxy, 4,
                  sizeof(vector<VsdCaloTower>), alignof(vector<VsdCaloTower>) );
      instance.SetNew(&new_vectorlEVsdCaloTowergR);
      instance.SetNewArray(&newArray_vectorlEVsdCaloTowergR);
      instance.SetDelete(&delete_vectorlEVsdCaloTowergR);
      instance.SetDeleteArray(&deleteArray_vectorlEVsdCaloTowergR);
      instance.SetDestructor(&destruct_vectorlEVsdCaloTowergR);
      static_assert(alignof(vector<VsdCaloTower>::value_type) <= 4096,
          "Class with alignment strictly greater than 4096 are currently not supported in CollectionProxy. "
          "Please report this case to the developers");
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<VsdCaloTower> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<VsdCaloTower>","std::vector<VsdCaloTower, std::allocator<VsdCaloTower> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<VsdCaloTower>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEVsdCaloTowergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<VsdCaloTower>*>(nullptr))->GetClass();
      vectorlEVsdCaloTowergR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEVsdCaloTowergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEVsdCaloTowergR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<VsdCaloTower> : new vector<VsdCaloTower>;
   }
   static void *newArray_vectorlEVsdCaloTowergR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<VsdCaloTower>[nElements] : new vector<VsdCaloTower>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEVsdCaloTowergR(void *p) {
      delete (static_cast<vector<VsdCaloTower>*>(p));
   }
   static void deleteArray_vectorlEVsdCaloTowergR(void *p) {
      delete [] (static_cast<vector<VsdCaloTower>*>(p));
   }
   static void destruct_vectorlEVsdCaloTowergR(void *p) {
      typedef vector<VsdCaloTower> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<VsdCaloTower>

namespace ROOT {
   // Registration Schema evolution read functions
   int RecordReadRules_VsdDict() {
      return 0;
   }
   static int _R__UNIQUE_DICT_(ReadRules_VsdDict) = RecordReadRules_VsdDict();R__UseDummy(_R__UNIQUE_DICT_(ReadRules_VsdDict));
} // namespace ROOT
namespace {
  void TriggerDictionaryInitialization_VsdDict_Impl() {
    static const char* headers[] = {
"VsdBase.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/fwdev/root-dev/may26/build/include",
"/home/fwdev/VSDNano/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "VsdDict dictionary forward declarations' payload"

#pragma diagnostic push
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class  __attribute__((annotate("$clingAutoload$VsdBase.h")))  VsdCaloTower;
namespace std{template <typename _Tp> class  __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class  __attribute__((annotate("$clingAutoload$VsdBase.h")))  VsdEventInfo;
class  __attribute__((annotate("$clingAutoload$VsdBase.h")))  VsdMuon;
class  __attribute__((annotate("$clingAutoload$VsdBase.h")))  VsdMET;
class  __attribute__((annotate("$clingAutoload$VsdBase.h")))  VsdJet;
class  __attribute__((annotate("$clingAutoload$VsdBase.h")))  VsdCandidate;
class  __attribute__((annotate("$clingAutoload$VsdBase.h")))  VsdVertex;
class  __attribute__((annotate("$clingAutoload$VsdBase.h")))  VsdBase;
#pragma diagnostic pop
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "VsdDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "VsdBase.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"VsdBase", payloadCode, "@",
"VsdCaloTower", payloadCode, "@",
"VsdCandidate", payloadCode, "@",
"VsdEventInfo", payloadCode, "@",
"VsdJet", payloadCode, "@",
"VsdMET", payloadCode, "@",
"VsdMuon", payloadCode, "@",
"VsdVertex", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("VsdDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_VsdDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_VsdDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_VsdDict() {
  TriggerDictionaryInitialization_VsdDict_Impl();
}
