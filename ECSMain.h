// GLOBAL VARIABLES

// Struct type to contain attributes of the 3 ECS states.
typedef struct ECSState_t {
	enum led {LED_GREEN, LED_YELLOW, LED_RED} led;
	enum sound {SOUND_OFF, SOUND_ON} sound;
	enum electrolysis {ELEC_OFF, ELEC_ON} electrolysis;
} ECSState_t;

// EM_NONE: Not currently editing anything
// EM_SOFT_LIMIT: Editing soft limit
// EM_HARD_LIMIT: Editing hard limit
typedef enum EditMode_t {EM_NONE, EM_SOFT_LIMIT, EM_HARD_LIMIT} EditMode_t;
extern EditMode_t ECSMain_EditMode;

extern const ECSState_t * ECSMain_State;
extern int ECSMain_CO2ADCVal; // 0 - 4095
extern int ECSMain_SoftLimit; // 0 - 999, resolution of .1
extern int ECSMain_HardLimit; // 0 - 999, resolution of .1
extern int ECSMain_CO2Val; // 0 - 999, resolution of .1
extern int ECSMain_PrevCO2ADCVal;
extern int ECSMain_PrevCO2Val;
extern int ECSMain_PrevSoftLimit;
extern int ECSMain_PrevHardLimit;
extern int ECSMain_PrevEditMode;

// PRIVATE FUNCTIONS

void initECSState(void);
void initECSModules(void);

void updatePrevValues(void);
void updateECSState(void);
void updateECSSystem(void);
void updateElectrolysis(void);
void updateLED(void);
void updateSound(void);

void DisableInterrupts(void);
void EnableInterrupts(void);

// TEST TYPES AND FUNCTIONS
typedef struct TestVars_t {
	int softLimit;
	int hardLimit;
	EditMode_t editMode;
	int co2Val;
	const ECSState_t * expectedState;
} TestVars_t;

void mainTest(void);
void testStates(void);
void testHelperOverrideVars(TestVars_t testVars);
static void testWait(void);
