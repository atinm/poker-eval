LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := poker-eval
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_SRC_FILES := \
	src/combinations.c \
	src/deck.c \
	src/deck_astud.c \
	src/deck_joker.c \
	src/deck_std.c \
	src/enumerate.c \
	src/enumord.c \
	src/evx.c \
	src/lowball.c \
	src/poker_wrapper.c \
	src/rules_astud.c \
	src/rules_joker.c \
	src/rules_std.c \
	src/t_astudcardmasks.c \
	src/t_botcard.c \
	src/t_botfivecards.c \
	src/t_botfivecardsj.c \
	src/t_cardmasks.c \
	src/t_evx_flushcards.c \
	src/t_evx_pairval.c \
	src/t_evx_strval.c \
	src/t_evx_tripsval.c \
	src/t_jokercardmasks.c \
	src/t_jokerstraight.c \
	src/t_maskrank.c \
	src/t_nbits.c \
	src/t_nbitsandstr.c \
	src/t_straight.c \
	src/t_topbit.c \
	src/t_topcard.c \
	src/t_topfivebits.c \
	src/t_topfivecards.c \
	src/t_toptwobits.c
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(BUILD_SHARED_LIBRARY)