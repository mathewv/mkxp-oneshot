#include "binding-util.h"
#include "binding-types.h"
#include "debugwriter.h"

#include "chromasdk/ChromaSDKImpl.h"

bool INIT_SUCCESS = false;

CChromaSDKImpl* chromaImpl = NULL;

void _attemptLinkSdk() {
  Debug() << "Attempting to bind Chroma SDK";
  #if !defined(_WIN32) && !defined(_WIN64)
    // currently razr chroma is windows-only.
    // so, if we are not building for windows platform
    // then don't bother with chroma stuff
    Debug() << "Chroma: Unsupported Platform";
    return;
  #endif

  chromaImpl = new CChromaSDKImpl();
  Debug() << "Chroma Impl @" << chromaImpl;
  chromaImpl->Initialize();
  INIT_SUCCESS = true;
}

RB_METHOD(chromaSetBase) {
  Debug() << "Chroma: Attempting to set base colour";
  	RB_UNUSED_PARAM;
    if (INIT_SUCCESS) {
      ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE effect = {};
      for (unsigned int y = 0; y < ChromaSDK::Keyboard::MAX_ROW; y++) {
        for (unsigned int x = 0; x < ChromaSDK::Keyboard::MAX_COLUMN; x++) {
          effect.Color[y][x] = RGB(0xFF, 0, 0xFF);
        }
      }
      chromaImpl->CreateKeyboardEffectImpl(
          ChromaSDK::Keyboard::CHROMA_CUSTOM,
          &effect,
          NULL);
      Debug() << "Chroma: Set base colour effect";
    }
    return Qnil;
}

void chromaBindingInit() {
  _attemptLinkSdk();

	VALUE module = rb_define_module("Chroma");
	_rb_define_module_function(module, "setBase", chromaSetBase);
}

void chromaBindingRelease() {
  if (chromaImpl) {
    chromaImpl->UnInitialize();
    delete chromaImpl;
    chromaImpl = NULL;
    INIT_SUCCESS = false;
  }
}
