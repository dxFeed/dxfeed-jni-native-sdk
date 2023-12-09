## Manual to build samples

### Samples
1. Build JAR `dxfeed-jni-native-sdk-0.1.0.jar` 
2. Build shared library `DxFeedJniNativeSdk.dylib|dll|.so`
3. Go to the sample directory (e.g. `DxFeedConnect`)
4. Create directory `build` and copy JAR and shared library there
5. Run script `build_release.sh|.cmd`