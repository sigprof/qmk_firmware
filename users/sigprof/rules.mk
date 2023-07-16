SRC += sigprof.c

DEFERRED_EXEC_ENABLE = yes

ifeq ($(strip $(CUSTOM_LANG_SWITCH_ENABLE)),yes)
    OPT_DEFS += -DCUSTOM_LANG_SWITCH_ENABLE
    SRC += lang_switch.c
endif
