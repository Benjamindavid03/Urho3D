#include "../Resource/BackgroundLoader.h"
#include "../Resource/Decompress.h"
#include "../Resource/Image.h"
#include "../Resource/JSONFile.h"
#include "../Resource/JSONValue.h"
#include "../Resource/Localization.h"
#include "../Resource/PListFile.h"
#include "../Resource/Resource.h"
#include "../Resource/ResourceCache.h"
#include "../Resource/ResourceEvents.h"
#include "../Resource/XMLElement.h"
#include "../Resource/XMLFile.h"

#include "../LuaScript/LuaScriptUtils.h"

#include <kaguya.hpp>

namespace Urho3D
{
static void RegisterImage(kaguya::State& lua)
{
    using namespace kaguya;

    lua["KCOLOR_LUT_SIZE"] = COLOR_LUT_SIZE;

    // enum CompressedFormat;
    lua["KCF_NONE"] = CF_NONE;
    lua["KCF_RGBA"] = CF_RGBA;
    lua["KCF_DXT1"] = CF_DXT1;
    lua["KCF_DXT3"] = CF_DXT3;
    lua["KCF_DXT5"] = CF_DXT5;
    lua["KCF_ETC1"] = CF_ETC1;
    lua["KCF_PVRTC_RGB_2BPP"] = CF_PVRTC_RGB_2BPP;
    lua["KCF_PVRTC_RGBA_2BPP"] = CF_PVRTC_RGBA_2BPP;
    lua["KCF_PVRTC_RGB_4BPP"] = CF_PVRTC_RGB_4BPP;
    lua["KCF_PVRTC_RGBA_4BPP"] = CF_PVRTC_RGBA_4BPP;

    lua["KImage"].setClass(UserdataMetatable<Image, Resource>(false)
        .addStaticFunction("new", &KCreateObject<Image>)
        .addStaticFunction("__gc", &KReleaseObject<Image>)

        .addFunction("GetType", &Image::GetType)
        .addFunction("GetTypeName", &Image::GetTypeName)
        .addFunction("GetTypeInfo", &Image::GetTypeInfo)
        .addStaticFunction("GetTypeStatic", &Image::GetTypeStatic)
        .addStaticFunction("GetTypeNameStatic", &Image::GetTypeNameStatic)
        .addStaticFunction("GetTypeInfoStatic", &Image::GetTypeInfoStatic)
        .addFunction("BeginLoad", &Image::BeginLoad)
        .addFunction("Save", &Image::Save)

        .addOverloadedFunctions("SetSize",
            static_cast<bool(Image::*)(int, int, unsigned)>(&Image::SetSize),
            static_cast<bool(Image::*)(int, int, int, unsigned)>(&Image::SetSize))

        .addOverloadedFunctions("SetPixel",
            static_cast<void(Image::*)(int, int, const Color&)>(&Image::SetPixel),
            static_cast<void(Image::*)(int, int, int, const Color&)>(&Image::SetPixel))

        .addOverloadedFunctions("SetPixelInt",
            static_cast<void(Image::*)(int, int, unsigned)>(&Image::SetPixelInt),
            static_cast<void(Image::*)(int, int, int, unsigned)>(&Image::SetPixelInt))

        .addFunction("LoadColorLUT", &Image::LoadColorLUT)
        .addFunction("FlipHorizontal", &Image::FlipHorizontal)
        .addFunction("FlipVertical", &Image::FlipVertical)
        .addFunction("Resize", &Image::Resize)
        .addFunction("Clear", &Image::Clear)
        .addFunction("ClearInt", &Image::ClearInt)
        .addFunction("SaveBMP", &Image::SaveBMP)
        .addFunction("SavePNG", &Image::SavePNG)
        .addFunction("SaveTGA", &Image::SaveTGA)
        .addFunction("SaveJPG", &Image::SaveJPG)
        .addFunction("IsCubemap", &Image::IsCubemap)
        .addFunction("IsArray", &Image::IsArray)
        .addFunction("IsSRGB", &Image::IsSRGB)

        .addOverloadedFunctions("GetPixel",
            static_cast<Color(Image::*)(int, int) const>(&Image::GetPixel),
            static_cast<Color(Image::*)(int, int, int) const>(&Image::GetPixel))

        .addOverloadedFunctions("GetPixelInt",
            static_cast<unsigned(Image::*)(int, int) const>(&Image::GetPixelInt),
            static_cast<unsigned(Image::*)(int, int, int) const>(&Image::GetPixelInt))

        .addFunction("GetPixelBilinear", &Image::GetPixelBilinear)
        .addFunction("GetPixelTrilinear", &Image::GetPixelTrilinear)
        .addFunction("GetWidth", &Image::GetWidth)
        .addFunction("GetHeight", &Image::GetHeight)
        .addFunction("GetDepth", &Image::GetDepth)
        .addFunction("GetComponents", &Image::GetComponents)
        .addFunction("GetData", &Image::GetData)
        .addFunction("IsCompressed", &Image::IsCompressed)
        .addFunction("GetCompressedFormat", &Image::GetCompressedFormat)
        .addFunction("GetNumCompressedLevels", &Image::GetNumCompressedLevels)
        .addFunction("GetNextLevel", &Image::GetNextLevel)
        .addFunction("GetNextSibling", &Image::GetNextSibling)
        .addFunction("ConvertToRGBA", &Image::ConvertToRGBA)
        .addFunction("GetCompressedLevel", &Image::GetCompressedLevel)
        .addFunction("GetSubimage", &Image::GetSubimage)
        .addFunction("GetSDLSurface", &Image::GetSDLSurface)
        .addFunction("PrecalculateLevels", &Image::PrecalculateLevels)

        .addProperty("type", &Image::GetType)
        .addProperty("typeName", &Image::GetTypeName)
        .addProperty("typeInfo", &Image::GetTypeInfo)
        .addProperty("cubemap", &Image::IsCubemap)
        .addProperty("array", &Image::IsArray)
        .addProperty("sRGB", &Image::IsSRGB)
        .addProperty("width", &Image::GetWidth)
        .addProperty("height", &Image::GetHeight)
        .addProperty("depth", &Image::GetDepth)
        .addProperty("components", &Image::GetComponents)
        .addProperty("data", &Image::GetData, &Image::SetData)
        .addProperty("compressed", &Image::IsCompressed)
        .addProperty("compressedFormat", &Image::GetCompressedFormat)
        .addProperty("numCompressedLevels", &Image::GetNumCompressedLevels)
        .addProperty("nextLevel", &Image::GetNextLevel)
        .addProperty("nextSibling", &Image::GetNextSibling)
    );
}
//
//static void RegisterJSONFile(kaguya::State& lua)
//{
//    using namespace kaguya;
//
//    lua["KJSONFile"].setClass(UserdataMetatable<JSONFile, Resource>(false)
//        .addStaticFunction("new", &KCreateObject<JSONFile>)
//        .addStaticFunction("__gc", &KReleaseObject<JSONFile>)
//
//        .addFunction("GetType", &JSONFile::GetType)
//        .addFunction("GetTypeName", &JSONFile::GetTypeName)
//        .addFunction("GetTypeInfo", &JSONFile::GetTypeInfo)
//        .addStaticFunction("GetTypeStatic", &JSONFile::GetTypeStatic)
//        .addStaticFunction("GetTypeNameStatic", &JSONFile::GetTypeNameStatic)
//        .addStaticFunction("GetTypeInfoStatic", &JSONFile::GetTypeInfoStatic)
//        .addFunction("BeginLoad", &JSONFile::BeginLoad)
//
//        .addOverloadedFunctions("Save",
//            static_cast<bool(JSONFile::*)(Serializer&) const>(&JSONFile::Save),
//            static_cast<bool(JSONFile::*)(Serializer&, const String&) const>(&JSONFile::Save))
//
//        .addFunction("FromString", &JSONFile::FromString)
//
//        .addOverloadedFunctions("GetRoot",
//            static_cast<JSONValue&(JSONFile::*)()>(&JSONFile::GetRoot),
//            static_cast<const JSONValue&(JSONFile::*)() const>(&JSONFile::GetRoot))
//
//
//        .addProperty("type", &JSONFile::GetType)
//        .addProperty("typeName", &JSONFile::GetTypeName)
//        .addProperty("typeInfo", &JSONFile::GetTypeInfo)
//    );
//}
//
//static void RegisterJSONValue(kaguya::State& lua)
//{
//    using namespace kaguya;
//
//    // enum JSONValueType;
//    lua["KJSON_NULL"] = JSON_NULL;
//    lua["KJSON_BOOL"] = JSON_BOOL;
//    lua["KJSON_NUMBER"] = JSON_NUMBER;
//    lua["KJSON_STRING"] = JSON_STRING;
//    lua["KJSON_ARRAY"] = JSON_ARRAY;
//    lua["KJSON_OBJECT"] = JSON_OBJECT;
//
//    // enum JSONNumberType;
//    lua["KJSONNT_NAN"] = JSONNT_NAN;
//    lua["KJSONNT_INT"] = JSONNT_INT;
//    lua["KJSONNT_UINT"] = JSONNT_UINT;
//    lua["KJSONNT_FLOAT_DOUBLE"] = JSONNT_FLOAT_DOUBLE;
//
//    lua["KJSONValue"] = JSONValue;
//}
//
//static void RegisterLocalization(kaguya::State& lua)
//{
//    using namespace kaguya;
//
//    lua["KLocalization"].setClass(UserdataMetatable<Localization, Object>(false)
//        .addStaticFunction("new", &KCreateObject<Localization>)
//        .addStaticFunction("__gc", &KReleaseObject<Localization>)
//
//        .addFunction("GetType", &Localization::GetType)
//        .addFunction("GetTypeName", &Localization::GetTypeName)
//        .addFunction("GetTypeInfo", &Localization::GetTypeInfo)
//        .addStaticFunction("GetTypeStatic", &Localization::GetTypeStatic)
//        .addStaticFunction("GetTypeNameStatic", &Localization::GetTypeNameStatic)
//        .addStaticFunction("GetTypeInfoStatic", &Localization::GetTypeInfoStatic)
//        .addFunction("GetNumLanguages", &Localization::GetNumLanguages)
//
//        .addOverloadedFunctions("GetLanguageIndex",
//            static_cast<int(Localization::*)() const>(&Localization::GetLanguageIndex),
//            static_cast<int(Localization::*)(const String&)>(&Localization::GetLanguageIndex))
//
//
//        .addOverloadedFunctions("GetLanguage",
//            static_cast<String(Localization::*)()>(&Localization::GetLanguage),
//            static_cast<String(Localization::*)(int)>(&Localization::GetLanguage))
//
//
//        .addOverloadedFunctions("SetLanguage",
//            static_cast<void(Localization::*)(int)>(&Localization::SetLanguage),
//            static_cast<void(Localization::*)(const String&)>(&Localization::SetLanguage))
//
//        .addFunction("Get", &Localization::Get)
//        .addFunction("Reset", &Localization::Reset)
//        .addFunction("LoadJSON", &Localization::LoadJSON)
//        .addFunction("LoadJSONFile", &Localization::LoadJSONFile)
//
//        .addProperty("type", &Localization::GetType)
//        .addProperty("typeName", &Localization::GetTypeName)
//        .addProperty("typeInfo", &Localization::GetTypeInfo)
//        .addProperty("numLanguages", &Localization::GetNumLanguages)
//    );
//}
//
//static void RegisterPListFile(kaguya::State& lua)
//{
//    using namespace kaguya;
//
//    // enum PListValueType;
//    lua["KPLVT_NONE"] = PLVT_NONE;
//    lua["KPLVT_INT"] = PLVT_INT;
//    lua["KPLVT_BOOL"] = PLVT_BOOL;
//    lua["KPLVT_FLOAT"] = PLVT_FLOAT;
//    lua["KPLVT_STRING"] = PLVT_STRING;
//    lua["KPLVT_VALUEMAP"] = PLVT_VALUEMAP;
//    lua["KPLVT_VALUEVECTOR"] = PLVT_VALUEVECTOR;
//
//    lua["KPListValue"].setClass(UserdataMetatable<PListValue>()
//        .setConstructors<PListValue(),
//            PListValue(int),
//            PListValue(bool),
//            PListValue(float),
//            PListValue(const String&),
//            PListValue(PListValueMap&),
//            PListValue(PListValueVector&),
//            PListValue(const PListValue&)>()
//
//        .addFunction("SetInt", &PListValue::SetInt)
//        .addFunction("SetBool", &PListValue::SetBool)
//        .addFunction("SetFloat", &PListValue::SetFloat)
//        .addFunction("SetString", &PListValue::SetString)
//        .addFunction("SetValueMap", &PListValue::SetValueMap)
//        .addFunction("SetValueVector", &PListValue::SetValueVector)
//        .addFunction("GetType", &PListValue::GetType)
//        .addFunction("GetInt", &PListValue::GetInt)
//        .addFunction("GetBool", &PListValue::GetBool)
//        .addFunction("GetFloat", &PListValue::GetFloat)
//        .addFunction("GetString", &PListValue::GetString)
//        .addFunction("GetIntRect", &PListValue::GetIntRect)
//        .addFunction("GetIntVector2", &PListValue::GetIntVector2)
//        .addFunction("GetValueMap", &PListValue::GetValueMap)
//        .addFunction("GetValueVector", &PListValue::GetValueVector)
//        .addFunction("ConvertToValueMap", &PListValue::ConvertToValueMap)
//        .addFunction("ConvertToValueVector", &PListValue::ConvertToValueVector)
//
//        .addProperty("type", &PListValue::GetType)
//        .addProperty("int", &PListValue::GetInt, &PListValue::SetInt)
//        .addProperty("bool", &PListValue::GetBool, &PListValue::SetBool)
//        .addProperty("float", &PListValue::GetFloat, &PListValue::SetFloat)
//        .addProperty("string", &PListValue::GetString, &PListValue::SetString)
//        .addProperty("intRect", &PListValue::GetIntRect)
//        .addProperty("intVector2", &PListValue::GetIntVector2)
//        .addProperty("valueMap", &PListValue::GetValueMap, &PListValue::SetValueMap)
//        .addProperty("valueVector", &PListValue::GetValueVector, &PListValue::SetValueVector)
//    );
//    lua["KPListFile"].setClass(UserdataMetatable<PListFile, Resource>(false)
//        .addStaticFunction("new", &KCreateObject<PListFile>)
//        .addStaticFunction("__gc", &KReleaseObject<PListFile>)
//
//        .addFunction("GetType", &PListFile::GetType)
//        .addFunction("GetTypeName", &PListFile::GetTypeName)
//        .addFunction("GetTypeInfo", &PListFile::GetTypeInfo)
//        .addStaticFunction("GetTypeStatic", &PListFile::GetTypeStatic)
//        .addStaticFunction("GetTypeNameStatic", &PListFile::GetTypeNameStatic)
//        .addStaticFunction("GetTypeInfoStatic", &PListFile::GetTypeInfoStatic)
//        .addFunction("BeginLoad", &PListFile::BeginLoad)
//        .addFunction("GetRoot", &PListFile::GetRoot)
//
//        .addProperty("type", &PListFile::GetType)
//        .addProperty("typeName", &PListFile::GetTypeName)
//        .addProperty("typeInfo", &PListFile::GetTypeInfo)
//        .addProperty("root", &PListFile::GetRoot)
//    );
//}
//
//static void RegisterResource(kaguya::State& lua)
//{
//    using namespace kaguya;
//
//    // enum AsyncLoadState;
//    lua["KASYNC_DONE"] = ASYNC_DONE;
//    lua["KASYNC_QUEUED"] = ASYNC_QUEUED;
//    lua["KASYNC_LOADING"] = ASYNC_LOADING;
//    lua["KASYNC_SUCCESS"] = ASYNC_SUCCESS;
//    lua["KASYNC_FAIL"] = ASYNC_FAIL;
//
//    lua["KResource"].setClass(UserdataMetatable<Resource, Object>(false)
//        .addStaticFunction("new", &KCreateObject<Resource>)
//        .addStaticFunction("__gc", &KReleaseObject<Resource>)
//
//        .addFunction("GetType", &Resource::GetType)
//        .addFunction("GetTypeName", &Resource::GetTypeName)
//        .addFunction("GetTypeInfo", &Resource::GetTypeInfo)
//        .addStaticFunction("GetTypeStatic", &Resource::GetTypeStatic)
//        .addStaticFunction("GetTypeNameStatic", &Resource::GetTypeNameStatic)
//        .addStaticFunction("GetTypeInfoStatic", &Resource::GetTypeInfoStatic)
//        .addFunction("Load", &Resource::Load)
//        .addFunction("BeginLoad", &Resource::BeginLoad)
//        .addFunction("EndLoad", &Resource::EndLoad)
//        .addFunction("Save", &Resource::Save)
//        .addFunction("SetName", &Resource::SetName)
//        .addFunction("SetMemoryUse", &Resource::SetMemoryUse)
//        .addFunction("ResetUseTimer", &Resource::ResetUseTimer)
//        .addFunction("SetAsyncLoadState", &Resource::SetAsyncLoadState)
//        .addFunction("GetName", &Resource::GetName)
//        .addFunction("GetNameHash", &Resource::GetNameHash)
//        .addFunction("GetMemoryUse", &Resource::GetMemoryUse)
//        .addFunction("GetUseTimer", &Resource::GetUseTimer)
//        .addFunction("GetAsyncLoadState", &Resource::GetAsyncLoadState)
//
//        .addProperty("type", &Resource::GetType)
//        .addProperty("typeName", &Resource::GetTypeName)
//        .addProperty("typeInfo", &Resource::GetTypeInfo)
//        .addProperty("name", &Resource::GetName, &Resource::SetName)
//        .addProperty("nameHash", &Resource::GetNameHash)
//        .addProperty("memoryUse", &Resource::GetMemoryUse, &Resource::SetMemoryUse)
//        .addProperty("useTimer", &Resource::GetUseTimer)
//        .addProperty("asyncLoadState", &Resource::GetAsyncLoadState, &Resource::SetAsyncLoadState)
//    );
//    lua["KGetResourceName"] = function(&GetResourceName);
//    lua["KGetResourceType"] = function(&GetResourceType);
//    lua["KGetResourceRef"] = function(&GetResourceRef);
//    // template <class T> Vector<String> GetResourceNames(const Vector<SharedPtr<T>>& resources);
//    // template <class T> ResourceRefList GetResourceRefList(const Vector<SharedPtr<T>>& resources);
//}
//
//static void RegisterResourceCache(kaguya::State& lua)
//{
//    using namespace kaguya;
//
//    lua["KPRIORITY_LAST"] = PRIORITY_LAST;
//    lua["KResourceGroup"].setClass(UserdataMetatable<ResourceGroup>()
//        .setConstructors<ResourceGroup()>()
//
//        .addProperty("memoryBudget", &ResourceGroup::memoryBudget_)
//        .addProperty("memoryUse", &ResourceGroup::memoryUse_)
//        .addProperty("resources", &ResourceGroup::resources_)
//    );
//    // enum ResourceRequest;
//    lua["KRESOURCE_CHECKEXISTS"] = RESOURCE_CHECKEXISTS;
//    lua["KRESOURCE_GETFILE"] = RESOURCE_GETFILE;
//
//    lua["KResourceRouter"].setClass(UserdataMetatable<ResourceRouter, Object>(false)
//        .addStaticFunction("new", &KCreateObject<ResourceRouter>)
//        .addStaticFunction("__gc", &KReleaseObject<ResourceRouter>)
//
//        .addFunction("Route", &ResourceRouter::Route)
//    );
//    lua["KResourceCache"].setClass(UserdataMetatable<ResourceCache, Object>(false)
//        .addStaticFunction("new", &KCreateObject<ResourceCache>)
//        .addStaticFunction("__gc", &KReleaseObject<ResourceCache>)
//
//        .addFunction("GetType", &ResourceCache::GetType)
//        .addFunction("GetTypeName", &ResourceCache::GetTypeName)
//        .addFunction("GetTypeInfo", &ResourceCache::GetTypeInfo)
//        .addStaticFunction("GetTypeStatic", &ResourceCache::GetTypeStatic)
//        .addStaticFunction("GetTypeNameStatic", &ResourceCache::GetTypeNameStatic)
//        .addStaticFunction("GetTypeInfoStatic", &ResourceCache::GetTypeInfoStatic)
//        .addFunction("AddResourceDir", &ResourceCache::AddResourceDir)
//
//        .addOverloadedFunctions("AddPackageFile",
//            static_cast<bool(ResourceCache::*)(PackageFile*, unsigned)>(&ResourceCache::AddPackageFile),
//            static_cast<bool(ResourceCache::*)(const String&, unsigned)>(&ResourceCache::AddPackageFile))
//
//        .addFunction("AddManualResource", &ResourceCache::AddManualResource)
//        .addFunction("RemoveResourceDir", &ResourceCache::RemoveResourceDir)
//
//        .addOverloadedFunctions("RemovePackageFile",
//            static_cast<void(ResourceCache::*)(PackageFile*, bool, bool)>(&ResourceCache::RemovePackageFile),
//            static_cast<void(ResourceCache::*)(const String&, bool, bool)>(&ResourceCache::RemovePackageFile))
//
//        .addFunction("ReleaseResource", &ResourceCache::ReleaseResource)
//
//        .addOverloadedFunctions("ReleaseResources",
//            static_cast<void(ResourceCache::*)(StringHash, bool)>(&ResourceCache::ReleaseResources),
//            static_cast<void(ResourceCache::*)(StringHash, const String&, bool)>(&ResourceCache::ReleaseResources),
//            static_cast<void(ResourceCache::*)(const String&, bool)>(&ResourceCache::ReleaseResources))
//
//        .addFunction("ReleaseAllResources", &ResourceCache::ReleaseAllResources)
//        .addFunction("ReloadResource", &ResourceCache::ReloadResource)
//        .addFunction("ReloadResourceWithDependencies", &ResourceCache::ReloadResourceWithDependencies)
//        .addFunction("SetMemoryBudget", &ResourceCache::SetMemoryBudget)
//        .addFunction("SetAutoReloadResources", &ResourceCache::SetAutoReloadResources)
//        .addFunction("SetReturnFailedResources", &ResourceCache::SetReturnFailedResources)
//        .addFunction("SetSearchPackagesFirst", &ResourceCache::SetSearchPackagesFirst)
//        .addFunction("SetFinishBackgroundResourcesMs", &ResourceCache::SetFinishBackgroundResourcesMs)
//        .addFunction("AddResourceRouter", &ResourceCache::AddResourceRouter)
//        .addFunction("RemoveResourceRouter", &ResourceCache::RemoveResourceRouter)
//        .addFunction("GetFile", &ResourceCache::GetFile)
//        .addFunction("GetResource", &ResourceCache::GetResource)
//        .addFunction("GetTempResource", &ResourceCache::GetTempResource)
//        .addFunction("BackgroundLoadResource", &ResourceCache::BackgroundLoadResource)
//        .addFunction("GetNumBackgroundLoadResources", &ResourceCache::GetNumBackgroundLoadResources)
//        .addFunction("GetResources", &ResourceCache::GetResources)
//        .addFunction("GetExistingResource", &ResourceCache::GetExistingResource)
//        .addFunction("GetAllResources", &ResourceCache::GetAllResources)
//        .addFunction("GetResourceDirs", &ResourceCache::GetResourceDirs)
//        .addFunction("GetPackageFiles", &ResourceCache::GetPackageFiles)
//        .addFunction("Exists", &ResourceCache::Exists)
//        .addFunction("GetMemoryBudget", &ResourceCache::GetMemoryBudget)
//        .addFunction("GetMemoryUse", &ResourceCache::GetMemoryUse)
//        .addFunction("GetTotalMemoryUse", &ResourceCache::GetTotalMemoryUse)
//        .addFunction("GetResourceFileName", &ResourceCache::GetResourceFileName)
//        .addFunction("GetAutoReloadResources", &ResourceCache::GetAutoReloadResources)
//        .addFunction("GetReturnFailedResources", &ResourceCache::GetReturnFailedResources)
//        .addFunction("GetSearchPackagesFirst", &ResourceCache::GetSearchPackagesFirst)
//        .addFunction("GetFinishBackgroundResourcesMs", &ResourceCache::GetFinishBackgroundResourcesMs)
//        .addFunction("GetResourceRouter", &ResourceCache::GetResourceRouter)
//        .addFunction("GetPreferredResourceDir", &ResourceCache::GetPreferredResourceDir)
//        .addFunction("SanitateResourceName", &ResourceCache::SanitateResourceName)
//        .addFunction("SanitateResourceDirName", &ResourceCache::SanitateResourceDirName)
//        .addFunction("StoreResourceDependency", &ResourceCache::StoreResourceDependency)
//        .addFunction("ResetDependencies", &ResourceCache::ResetDependencies)
//        .addFunction("PrintMemoryUsage", &ResourceCache::PrintMemoryUsage)
//
//        .addProperty("type", &ResourceCache::GetType)
//        .addProperty("typeName", &ResourceCache::GetTypeName)
//        .addProperty("typeInfo", &ResourceCache::GetTypeInfo)
//        .addProperty("numBackgroundLoadResources", &ResourceCache::GetNumBackgroundLoadResources)
//        .addProperty("allResources", &ResourceCache::GetAllResources)
//        .addProperty("resourceDirs", &ResourceCache::GetResourceDirs)
//        .addProperty("packageFiles", &ResourceCache::GetPackageFiles)
//        .addProperty("totalMemoryUse", &ResourceCache::GetTotalMemoryUse)
//        .addProperty("autoReloadResources", &ResourceCache::GetAutoReloadResources, &ResourceCache::SetAutoReloadResources)
//        .addProperty("returnFailedResources", &ResourceCache::GetReturnFailedResources, &ResourceCache::SetReturnFailedResources)
//        .addProperty("searchPackagesFirst", &ResourceCache::GetSearchPackagesFirst, &ResourceCache::SetSearchPackagesFirst)
//        .addProperty("finishBackgroundResourcesMs", &ResourceCache::GetFinishBackgroundResourcesMs, &ResourceCache::SetFinishBackgroundResourcesMs)
//    );
//    // template <class T> T* ResourceCache::GetExistingResource(const String& name);
//    // template <class T> T* ResourceCache::GetResource(const String& name, bool sendEventOnFailure);
//    // template <class T> void ResourceCache::ReleaseResource(const String& name, bool force);
//    // template <class T> SharedPtr<T> ResourceCache::GetTempResource(const String& name, bool sendEventOnFailure);
//    // template <class T> bool ResourceCache::BackgroundLoadResource(const String& name, bool sendEventOnFailure, Resource* caller);
//    // template <class T> void ResourceCache::GetResources(PODVector<T*>& result);
//    lua["KRegisterResourceLibrary"] = function(&RegisterResourceLibrary);
//}
//
//static void RegisterResourceEvents(kaguya::State& lua)
//{
//    using namespace kaguya;
//
//    lua["KE_RELOADSTARTED"] = E_RELOADSTARTED;
//    lua["KE_RELOADFINISHED"] = E_RELOADFINISHED;
//    lua["KE_RELOADFAILED"] = E_RELOADFAILED;
//    lua["KE_FILECHANGED"] = E_FILECHANGED;
//    lua["KE_LOADFAILED"] = E_LOADFAILED;
//    lua["KE_RESOURCENOTFOUND"] = E_RESOURCENOTFOUND;
//    lua["KE_UNKNOWNRESOURCETYPE"] = E_UNKNOWNRESOURCETYPE;
//    lua["KE_RESOURCEBACKGROUNDLOADED"] = E_RESOURCEBACKGROUNDLOADED;
//    lua["KE_CHANGELANGUAGE"] = E_CHANGELANGUAGE;
//}
//
//static void RegisterXMLElement(kaguya::State& lua)
//{
//    using namespace kaguya;
//
//    lua["KXMLElement"].setClass(UserdataMetatable<XMLElement>()
//        .setConstructors<XMLElement(),
//            XMLElement(XMLFile*, pugi::xml_node_struct*),
//            XMLElement(XMLFile*, const XPathResultSet*, const pugi::xpath_node*, unsigned),
//            XMLElement(const XMLElement&)>()
//
//
//        .addOverloadedFunctions("CreateChild",
//            static_cast<XMLElement(XMLElement::*)(const String&)>(&XMLElement::CreateChild),
//            static_cast<XMLElement(XMLElement::*)(const char*)>(&XMLElement::CreateChild))
//
//
//        .addOverloadedFunctions("RemoveChild",
//            static_cast<bool(XMLElement::*)(const XMLElement&)>(&XMLElement::RemoveChild),
//            static_cast<bool(XMLElement::*)(const String&)>(&XMLElement::RemoveChild),
//            static_cast<bool(XMLElement::*)(const char*)>(&XMLElement::RemoveChild))
//
//
//        .addOverloadedFunctions("RemoveChildren",
//            static_cast<bool(XMLElement::*)(const String&)>(&XMLElement::RemoveChildren),
//            static_cast<bool(XMLElement::*)(const char*)>(&XMLElement::RemoveChildren))
//
//
//        .addOverloadedFunctions("RemoveAttribute",
//            static_cast<bool(XMLElement::*)(const String&)>(&XMLElement::RemoveAttribute),
//            static_cast<bool(XMLElement::*)(const char*)>(&XMLElement::RemoveAttribute))
//
//        .addFunction("SelectSingle", &XMLElement::SelectSingle)
//        .addFunction("SelectSinglePrepared", &XMLElement::SelectSinglePrepared)
//        .addFunction("Select", &XMLElement::Select)
//        .addFunction("SelectPrepared", &XMLElement::SelectPrepared)
//
//        .addOverloadedFunctions("SetValue",
//            static_cast<bool(XMLElement::*)(const String&)>(&XMLElement::SetValue),
//            static_cast<bool(XMLElement::*)(const char*)>(&XMLElement::SetValue))
//
//
//        .addOverloadedFunctions("SetAttribute",
//            static_cast<bool(XMLElement::*)(const String&, const String&)>(&XMLElement::SetAttribute),
//            static_cast<bool(XMLElement::*)(const char*, const char*)>(&XMLElement::SetAttribute),
//            static_cast<bool(XMLElement::*)(const String&)>(&XMLElement::SetAttribute),
//            static_cast<bool(XMLElement::*)(const char*)>(&XMLElement::SetAttribute))
//
//        .addFunction("SetBool", &XMLElement::SetBool)
//        .addFunction("SetBoundingBox", &XMLElement::SetBoundingBox)
//
//        .addOverloadedFunctions("SetBuffer",
//            static_cast<bool(XMLElement::*)(const String&, const void*, unsigned)>(&XMLElement::SetBuffer),
//            static_cast<bool(XMLElement::*)(const String&, const PODVector<unsigned char>&)>(&XMLElement::SetBuffer))
//
//        .addFunction("SetColor", &XMLElement::SetColor)
//        .addFunction("SetFloat", &XMLElement::SetFloat)
//        .addFunction("SetDouble", &XMLElement::SetDouble)
//        .addFunction("SetUInt", &XMLElement::SetUInt)
//        .addFunction("SetInt", &XMLElement::SetInt)
//        .addFunction("SetIntRect", &XMLElement::SetIntRect)
//        .addFunction("SetIntVector2", &XMLElement::SetIntVector2)
//        .addFunction("SetRect", &XMLElement::SetRect)
//        .addFunction("SetQuaternion", &XMLElement::SetQuaternion)
//        .addFunction("SetString", &XMLElement::SetString)
//        .addFunction("SetVariant", &XMLElement::SetVariant)
//        .addFunction("SetVariantValue", &XMLElement::SetVariantValue)
//        .addFunction("SetResourceRef", &XMLElement::SetResourceRef)
//        .addFunction("SetResourceRefList", &XMLElement::SetResourceRefList)
//        .addFunction("SetVariantVector", &XMLElement::SetVariantVector)
//        .addFunction("SetStringVector", &XMLElement::SetStringVector)
//        .addFunction("SetVariantMap", &XMLElement::SetVariantMap)
//        .addFunction("SetVector2", &XMLElement::SetVector2)
//        .addFunction("SetVector3", &XMLElement::SetVector3)
//        .addFunction("SetVector4", &XMLElement::SetVector4)
//        .addFunction("SetVectorVariant", &XMLElement::SetVectorVariant)
//        .addFunction("SetMatrix3", &XMLElement::SetMatrix3)
//        .addFunction("SetMatrix3x4", &XMLElement::SetMatrix3x4)
//        .addFunction("SetMatrix4", &XMLElement::SetMatrix4)
//        .addFunction("IsNull", &XMLElement::IsNull)
//        .addFunction("NotNull", &XMLElement::NotNull)
//        .addFunction("GetName", &XMLElement::GetName)
//
//        .addOverloadedFunctions("HasChild",
//            static_cast<bool(XMLElement::*)(const String&) const>(&XMLElement::HasChild),
//            static_cast<bool(XMLElement::*)(const char*) const>(&XMLElement::HasChild))
//
//
//        .addOverloadedFunctions("GetChild",
//            static_cast<XMLElement(XMLElement::*)(const String&) const>(&XMLElement::GetChild),
//            static_cast<XMLElement(XMLElement::*)(const char*) const>(&XMLElement::GetChild))
//
//
//        .addOverloadedFunctions("GetNext",
//            static_cast<XMLElement(XMLElement::*)(const String&) const>(&XMLElement::GetNext),
//            static_cast<XMLElement(XMLElement::*)(const char*) const>(&XMLElement::GetNext))
//
//        .addFunction("GetParent", &XMLElement::GetParent)
//        .addFunction("GetNumAttributes", &XMLElement::GetNumAttributes)
//
//        .addOverloadedFunctions("HasAttribute",
//            static_cast<bool(XMLElement::*)(const String&) const>(&XMLElement::HasAttribute),
//            static_cast<bool(XMLElement::*)(const char*) const>(&XMLElement::HasAttribute))
//
//        .addFunction("GetValue", &XMLElement::GetValue)
//
//        .addOverloadedFunctions("GetAttribute",
//            static_cast<String(XMLElement::*)(const String&) const>(&XMLElement::GetAttribute),
//            static_cast<String(XMLElement::*)(const char*) const>(&XMLElement::GetAttribute))
//
//        .addFunction("GetAttributeCString", &XMLElement::GetAttributeCString)
//
//        .addOverloadedFunctions("GetAttributeLower",
//            static_cast<String(XMLElement::*)(const String&) const>(&XMLElement::GetAttributeLower),
//            static_cast<String(XMLElement::*)(const char*) const>(&XMLElement::GetAttributeLower))
//
//
//        .addOverloadedFunctions("GetAttributeUpper",
//            static_cast<String(XMLElement::*)(const String&) const>(&XMLElement::GetAttributeUpper),
//            static_cast<String(XMLElement::*)(const char*) const>(&XMLElement::GetAttributeUpper))
//
//        .addFunction("GetAttributeNames", &XMLElement::GetAttributeNames)
//        .addFunction("GetBool", &XMLElement::GetBool)
//
//        .addOverloadedFunctions("GetBuffer",
//            static_cast<PODVector<unsigned char>(XMLElement::*)(const String&) const>(&XMLElement::GetBuffer),
//            static_cast<bool(XMLElement::*)(const String&, void*, unsigned) const>(&XMLElement::GetBuffer))
//
//        .addFunction("GetBoundingBox", &XMLElement::GetBoundingBox)
//        .addFunction("GetColor", &XMLElement::GetColor)
//        .addFunction("GetFloat", &XMLElement::GetFloat)
//        .addFunction("GetDouble", &XMLElement::GetDouble)
//        .addFunction("GetUInt", &XMLElement::GetUInt)
//        .addFunction("GetInt", &XMLElement::GetInt)
//        .addFunction("GetIntRect", &XMLElement::GetIntRect)
//        .addFunction("GetIntVector2", &XMLElement::GetIntVector2)
//        .addFunction("GetRect", &XMLElement::GetRect)
//        .addFunction("GetQuaternion", &XMLElement::GetQuaternion)
//        .addFunction("GetVariant", &XMLElement::GetVariant)
//        .addFunction("GetVariantValue", &XMLElement::GetVariantValue)
//        .addFunction("GetResourceRef", &XMLElement::GetResourceRef)
//        .addFunction("GetResourceRefList", &XMLElement::GetResourceRefList)
//        .addFunction("GetVariantVector", &XMLElement::GetVariantVector)
//        .addFunction("GetStringVector", &XMLElement::GetStringVector)
//        .addFunction("GetVariantMap", &XMLElement::GetVariantMap)
//        .addFunction("GetVector2", &XMLElement::GetVector2)
//        .addFunction("GetVector3", &XMLElement::GetVector3)
//        .addFunction("GetVector4", &XMLElement::GetVector4)
//        .addFunction("GetVector", &XMLElement::GetVector)
//        .addFunction("GetVectorVariant", &XMLElement::GetVectorVariant)
//        .addFunction("GetMatrix3", &XMLElement::GetMatrix3)
//        .addFunction("GetMatrix3x4", &XMLElement::GetMatrix3x4)
//        .addFunction("GetMatrix4", &XMLElement::GetMatrix4)
//        .addFunction("GetFile", &XMLElement::GetFile)
//        .addFunction("GetNode", &XMLElement::GetNode)
//        .addFunction("GetXPathResultSet", &XMLElement::GetXPathResultSet)
//        .addFunction("GetXPathNode", &XMLElement::GetXPathNode)
//        .addFunction("GetXPathResultIndex", &XMLElement::GetXPathResultIndex)
//        .addFunction("NextResult", &XMLElement::NextResult)
//
//        .addProperty("null", &XMLElement::IsNull)
//        .addProperty("name", &XMLElement::GetName)
//        .addProperty("parent", &XMLElement::GetParent)
//        .addProperty("numAttributes", &XMLElement::GetNumAttributes)
//        .addProperty("value", &XMLElement::GetValue)
//        .addProperty("attributeNames", &XMLElement::GetAttributeNames)
//        .addProperty("boundingBox", &XMLElement::GetBoundingBox, &XMLElement::SetBoundingBox)
//        .addProperty("variant", &XMLElement::GetVariant, &XMLElement::SetVariant)
//        .addProperty("resourceRef", &XMLElement::GetResourceRef, &XMLElement::SetResourceRef)
//        .addProperty("resourceRefList", &XMLElement::GetResourceRefList, &XMLElement::SetResourceRefList)
//        .addProperty("variantVector", &XMLElement::GetVariantVector, &XMLElement::SetVariantVector)
//        .addProperty("stringVector", &XMLElement::GetStringVector, &XMLElement::SetStringVector)
//        .addProperty("variantMap", &XMLElement::GetVariantMap, &XMLElement::SetVariantMap)
//        .addProperty("file", &XMLElement::GetFile)
//        .addProperty("node", &XMLElement::GetNode)
//        .addProperty("xPathResultSet", &XMLElement::GetXPathResultSet)
//        .addProperty("xPathNode", &XMLElement::GetXPathNode)
//        .addProperty("xPathResultIndex", &XMLElement::GetXPathResultIndex)
//        .addProperty("variantValue", &XMLElement::SetVariantValue)
//        .addStaticField("EMPTY", &XMLElement::EMPTY)
//    );
//    lua["KXPathResultSet"].setClass(UserdataMetatable<XPathResultSet>()
//        .setConstructors<XPathResultSet(),
//            XPathResultSet(XMLFile*, pugi::xpath_node_set*),
//            XPathResultSet(const XPathResultSet&)>()
//
//        .addFunction("operator[]", &XPathResultSet::operator[])
//        .addFunction("FirstResult", &XPathResultSet::FirstResult)
//        .addFunction("Size", &XPathResultSet::Size)
//        .addFunction("Empty", &XPathResultSet::Empty)
//        .addFunction("GetXPathNodeSet", &XPathResultSet::GetXPathNodeSet)
//
//        .addProperty("xPathNodeSet", &XPathResultSet::GetXPathNodeSet)
//    );
//    lua["KXPathQuery"].setClass(UserdataMetatable<XPathQuery>()
//        .setConstructors<XPathQuery(),
//            XPathQuery(const String&, const String&)>()
//
//        .addFunction("Bind", &XPathQuery::Bind)
//
//        .addOverloadedFunctions("SetVariable",
//            static_cast<bool(XPathQuery::*)(const String&, bool)>(&XPathQuery::SetVariable),
//            static_cast<bool(XPathQuery::*)(const String&, float)>(&XPathQuery::SetVariable),
//            static_cast<bool(XPathQuery::*)(const String&, const String&)>(&XPathQuery::SetVariable),
//            static_cast<bool(XPathQuery::*)(const char*, const char*)>(&XPathQuery::SetVariable),
//            static_cast<bool(XPathQuery::*)(const String&, const XPathResultSet&)>(&XPathQuery::SetVariable))
//
//        .addFunction("SetQuery", &XPathQuery::SetQuery)
//        .addFunction("Clear", &XPathQuery::Clear)
//        .addFunction("EvaluateToBool", &XPathQuery::EvaluateToBool)
//        .addFunction("EvaluateToFloat", &XPathQuery::EvaluateToFloat)
//        .addFunction("EvaluateToString", &XPathQuery::EvaluateToString)
//        .addFunction("Evaluate", &XPathQuery::Evaluate)
//        .addFunction("GetQuery", &XPathQuery::GetQuery)
//        .addFunction("GetXPathQuery", &XPathQuery::GetXPathQuery)
//        .addFunction("GetXPathVariableSet", &XPathQuery::GetXPathVariableSet)
//
//        .addProperty("query", &XPathQuery::GetQuery)
//        .addProperty("xPathQuery", &XPathQuery::GetXPathQuery)
//        .addProperty("xPathVariableSet", &XPathQuery::GetXPathVariableSet)
//    );
//}
//
//static void RegisterXMLFile(kaguya::State& lua)
//{
//    using namespace kaguya;
//
//    lua["KXMLFile"].setClass(UserdataMetatable<XMLFile, Resource>(false)
//        .addStaticFunction("new", &KCreateObject<XMLFile>)
//        .addStaticFunction("__gc", &KReleaseObject<XMLFile>)
//
//        .addFunction("GetType", &XMLFile::GetType)
//        .addFunction("GetTypeName", &XMLFile::GetTypeName)
//        .addFunction("GetTypeInfo", &XMLFile::GetTypeInfo)
//        .addStaticFunction("GetTypeStatic", &XMLFile::GetTypeStatic)
//        .addStaticFunction("GetTypeNameStatic", &XMLFile::GetTypeNameStatic)
//        .addStaticFunction("GetTypeInfoStatic", &XMLFile::GetTypeInfoStatic)
//        .addFunction("BeginLoad", &XMLFile::BeginLoad)
//
//        .addOverloadedFunctions("Save",
//            static_cast<bool(XMLFile::*)(Serializer&) const>(&XMLFile::Save),
//            static_cast<bool(XMLFile::*)(Serializer&, const String&) const>(&XMLFile::Save))
//
//        .addFunction("FromString", &XMLFile::FromString)
//        .addFunction("CreateRoot", &XMLFile::CreateRoot)
//        .addFunction("GetRoot", &XMLFile::GetRoot)
//        .addFunction("GetDocument", &XMLFile::GetDocument)
//        .addFunction("ToString", &XMLFile::ToString)
//
//        .addOverloadedFunctions("Patch",
//            static_cast<void(XMLFile::*)(XMLFile*)>(&XMLFile::Patch),
//            static_cast<void(XMLFile::*)(XMLElement)>(&XMLFile::Patch))
//
//
//        .addProperty("type", &XMLFile::GetType)
//        .addProperty("typeName", &XMLFile::GetTypeName)
//        .addProperty("typeInfo", &XMLFile::GetTypeInfo)
//        .addProperty("document", &XMLFile::GetDocument)
//    );
//}
//
void RegisterResourceLuaAPI(kaguya::State& lua)
{
    RegisterImage(lua);
    /*
    RegisterJSONFile(lua);
    RegisterJSONValue(lua);
    RegisterLocalization(lua);
    RegisterPListFile(lua);
    RegisterResource(lua);
    RegisterResourceCache(lua);
    RegisterResourceEvents(lua);
    RegisterXMLElement(lua);
    RegisterXMLFile(lua);
    */
}
}