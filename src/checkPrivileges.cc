#include <napi.h>
#include <windows.h>  
#include <lm.h> 

#pragma comment(lib, "netapi32.lib")  

Napi::String CheckPrivileges(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();  // Окружение N-API
  std::string privilege;

  if (info.Length() < 1 || !info[0].IsString()) {
    Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    return Napi::String::New(env, "");  
  }

  std::string username = info[0].As<Napi::String>().Utf8Value();
  std::wstring wusername(username.begin(), username.end());  

  // Указатель на структуру USER_INFO_1, которая будет содержать информацию о пользователе
  LPUSER_INFO_1 pBuf = NULL; 
  // 1 - базовая информация 
  DWORD dwLevel = 1;  
  DWORD dwError = 0;  

  NET_API_STATUS nStatus = NetUserGetInfo(NULL, wusername.c_str(), dwLevel, (LPBYTE*)&pBuf);

  if (nStatus == NERR_Success) {
      
    if (pBuf->usri1_priv == USER_PRIV_ADMIN) { 
      privilege = "Администратор";
    } else if (pBuf->usri1_priv == USER_PRIV_USER) { 
      privilege = "Пользователь";
    } else {  
      privilege = "Гость";
    }
    // Освобождение буфера
    NetApiBufferFree(pBuf);  
    return Napi::String::New(env, privilege); 
  } else {
    return Napi::String::New(env, ""); 
  }
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "checkPrivileges"), Napi::Function::New(env, CheckPrivileges));
  return exports;
}

NODE_API_MODULE(checkPrivileges, Init)