LOCAL_PATH := $(call my-dir)/../..

include $(CLEAR_VARS)

LOCAL_MODULE := ZgeSkelet

MY_CAL3D_PATH := cal3d/src
MY_CAL3D_CPP_PATH := $(MY_CAL3D_PATH)/cal3d
LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(MY_CAL3D_PATH)
LOCAL_CPPFLAGS += -fexceptions -frtti -DCAL_16BIT_INDICES
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -ldl -lm -lstdc++ -lGLESv1_CM

# uncomment for ARMv7-A
LOCAL_CFLAGS := -march=armv7-a -mfloat-abi=softfp

TARGET_PLATFORM := android-9

LOCAL_SRC_FILES := \
	src/ZgeSkelet.cpp\
	$(MY_CAL3D_CPP_PATH)/animation.cpp\
	$(MY_CAL3D_CPP_PATH)/animation_action.cpp\
	$(MY_CAL3D_CPP_PATH)/animation_cycle.cpp\
	$(MY_CAL3D_CPP_PATH)/bone.cpp\
	$(MY_CAL3D_CPP_PATH)/buffersource.cpp\
	$(MY_CAL3D_CPP_PATH)/cal3d_wrapper.cpp\
	$(MY_CAL3D_CPP_PATH)/coreanimatedmorph.cpp\
	$(MY_CAL3D_CPP_PATH)/coreanimation.cpp\
	$(MY_CAL3D_CPP_PATH)/corebone.cpp\
	$(MY_CAL3D_CPP_PATH)/corekeyframe.cpp\
	$(MY_CAL3D_CPP_PATH)/corematerial.cpp\
	$(MY_CAL3D_CPP_PATH)/coremesh.cpp\
	$(MY_CAL3D_CPP_PATH)/coremodel.cpp\
	$(MY_CAL3D_CPP_PATH)/coremorphkeyframe.cpp\
	$(MY_CAL3D_CPP_PATH)/coremorphtrack.cpp\
	$(MY_CAL3D_CPP_PATH)/coreskeleton.cpp\
	$(MY_CAL3D_CPP_PATH)/coresubmesh.cpp\
	$(MY_CAL3D_CPP_PATH)/coresubmorphtarget.cpp\
	$(MY_CAL3D_CPP_PATH)/coretrack.cpp\
	$(MY_CAL3D_CPP_PATH)/dualquaternion.cpp\
	$(MY_CAL3D_CPP_PATH)/error.cpp\
	$(MY_CAL3D_CPP_PATH)/global.cpp\
	$(MY_CAL3D_CPP_PATH)/hardwaremodel.cpp\
	$(MY_CAL3D_CPP_PATH)/loader.cpp\
	$(MY_CAL3D_CPP_PATH)/matrix.cpp\
	$(MY_CAL3D_CPP_PATH)/mesh.cpp\
	$(MY_CAL3D_CPP_PATH)/mixer.cpp\
	$(MY_CAL3D_CPP_PATH)/model.cpp\
	$(MY_CAL3D_CPP_PATH)/morphtargetmixer.cpp\
	$(MY_CAL3D_CPP_PATH)/physique.cpp\
	$(MY_CAL3D_CPP_PATH)/physiquedualquaternion.cpp\
	$(MY_CAL3D_CPP_PATH)/platform.cpp\
	$(MY_CAL3D_CPP_PATH)/quaternion.cpp\
	$(MY_CAL3D_CPP_PATH)/renderer.cpp\
	$(MY_CAL3D_CPP_PATH)/saver.cpp\
	$(MY_CAL3D_CPP_PATH)/skeleton.cpp\
	$(MY_CAL3D_CPP_PATH)/springsystem.cpp\
	$(MY_CAL3D_CPP_PATH)/streamsource.cpp\
	$(MY_CAL3D_CPP_PATH)/submesh.cpp\
	$(MY_CAL3D_CPP_PATH)/tinyxml.cpp\
	$(MY_CAL3D_CPP_PATH)/tinyxmlerror.cpp\
	$(MY_CAL3D_CPP_PATH)/tinyxmlparser.cpp\
	$(MY_CAL3D_CPP_PATH)/vector.cpp\
	$(MY_CAL3D_CPP_PATH)/xmlformat.cpp

include $(BUILD_SHARED_LIBRARY)
