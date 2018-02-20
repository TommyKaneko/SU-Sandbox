#include <iostream>
#include <stdexcept>
#include <cassert>
#include <string>
#include <vector>
#include <SketchUpAPI/sketchup.h>

#define SU_CALL(func) {SUResult res = (func); if (res != SU_ERROR_NONE) throw std::runtime_error(std::to_string(res));}

int main() {

  SUInitialize();
  // Load the model from a file
  SUModelRef model = SU_INVALID;
  SUResult res = SUModelCreateFromFile(&model, "/Users/tomkaneko/Dropbox/Sketchup/Extensions/SU-Sandbox/src/SUTypedValueGetArrayItems_test/issue-19.skp"); // Adjust according to your file system.
  assert(res == SU_ERROR_NONE);
  SUAttributeDictionaryRef dict_ref =  SU_INVALID ;
  res = SUModelGetAttributeDictionary(model, "TypedValues", &dict_ref);
  
  SUTypedValueRef val = SU_INVALID;
  SUTypedValueCreate(&val);
  const char* key_char = "Array";
  res = SUAttributeDictionaryGetValue(dict_ref, &key_char[0], &val);
  assert(res == SU_ERROR_NONE);
  size_t count = 0;
  res = SUTypedValueGetNumArrayItems(val, &count);
  assert(res == SU_ERROR_NONE);
  SUTypedValueRef* values = new SUTypedValueRef[count];
  res = SUTypedValueGetArrayItems(val, count, &values[0], &count);
  assert(res == SU_ERROR_NONE);
  for (size_t i=0; i < count; ++i) {
    // Do stuff with the typed Values
    SUTypedValueType type;
    res = SUTypedValueGetType(values[i], &type);
    std::cout << "TypedValue type is No: " << type << "\n";
    // Then release each one
    SUTypedValueRelease(&values[i]); // CRASH on second pass - indicates TypedValue has already been released.
  }
  delete values;
  res = SUModelRelease(&model);
  SUTerminate();

  return 0;
}

