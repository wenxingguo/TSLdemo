/**************************************************
 * 该文件由 TSLParserTest生成
 **************************************************/
#include <string>
#include <vector>

enum class batteryState_enum { //电池当前状态 电池状态
    NA = 0,
    CHARGING = 2,
    NO_CHARGING = 3
};

enum class emergencyState_enum { //急停状态 急停状态
    STATE_EMERGENCY_NA = 0,
    STATE_EMERGENCY_NONE = 1,
    STATE_EMERGENCY_TRIGER = 2,
    STATE_EMERGENCY_RECOVERABLE = 3
};

enum class breakSwState_enum { //解抱闸状态 解抱闸状态
    NA = 0,
    OFF = 1,
    ON = 2
};

enum class sysState_enum { //系统状态 系统状态
    SYS_STATE_ZERO = 0,
    SYS_STATE_INITIALING = 1,
    SYS_STATE_IDLE = 2,
    SYS_STATE_ERROR = 3,
    SYS_STATE_START_LOCATING = 4,
    SYS_STATE_TASK_NAV_INITIALING = 5,
    SYS_STATE_TASK_NAV_FINDING_PATH = 6,
    SYS_STATE_TASK_NAV_WAITING_FINISH = 7,
    SYS_STATE_TASK_NAV_WAITING_FINISH_SLOW = 8,
    SYS_STATE_TASK_NAV_REFINDING_PATH = 9,
    SYS_STATE_TASK_NAV_PAUSED = 10,
    SYS_STATE_TASK_NAV_NO_WAY = 11,
    SYS_STATE_TASK_NEWMAP_DRAWING = 12,
    SYS_STATE_TASK_NEWMAP_SAVING = 13,
    SYS_STATE_TASK_PATH_NAV_INITIALING = 14,
    SYS_STATE_TASK_PATH_WAITING_FINISH = 15,
    SYS_STATE_TASK_PATH_WAITING_FINISH_SLOW = 16,
    SYS_STATE_TASK_PATH_PAUSED = 17,
    SYS_STATE_TASK_NAV_NO_STATION = 18,
    SYS_STATE_TASK_MANUAL_PAUSED = 19,
    SYS_STATE_TASK_NAV_PATH_ERROR = 20,
    SYS_STATE_TASK_MANUAL_PATH_ERROR = 21,
    SYS_STATE_HARDWARE_ERROR = 22,
    SYS_STATE_TASK_PATH_WAITING_TRAFFIC_CONTROL = 23,
    SYS_STATE_TASK_PATH_WAITING_TRAFFIC_CONTROL_SLOW = 24
};

enum class locationState_enum { //定位状态 定位状态
    LOCATION_STATE_ZERO = 0,
    LOCATION_STATE_NONE = 1,
    LOCATION_STATE_INITIALING = 2,
    LOCATION_STATE_RUNNING = 3,
    LOCATION_STATE_RELOCATING = 4,
    LOCATION_STATE_ERROR = 5
};

struct currentPosition_struct { // 当前定位 当前定位
    int x; // x坐标

    int y; // y坐标

    int z; // z坐标

    int roll; // roll

    int pitch; // pitch

    int yaw; // yaw

    int confidence; // confidence
};

enum class state_enum { // TaskState
    MT_NA = 0,
    MT_WAIT_FOR_START = 2,
    MT_RUNNING = 3,
    MT_PAUSED = 4,
    MT_FINISHED = 5,
    MT_IN_CANCEL = 6,
    MT_TASK_WAIT_FOR_ACK = 7,
    MT_WAIT_FOR_CHECKPOINT = 8
};

enum class type_enum { // TaskType
    MT_ZERO = 0,
    MT_MOVE_FOLLOW_PATH = 1,
    MT_MOVE_TO_STATION = 2,
    MT_MOVE_TO_POSE = 3,
    MT_MOVE_MIX2 = 4
};

struct poses_array_item_struct { //
    int x; // 位置-x坐标

    int y; // 位置-y坐标

    int z; // 位置-z坐标

    int roll; // roll

    int pitch; // pitch

    int yaw; // yaw

    int confidence; // confidence
};

enum class pathType_enum { // PathType
    PATH_ZERO = 0,
    PATH_LINE = 1,
    PATH_CIRCLE = 2,
    PATH_BEZIER = 3,
    PATH_ROTATE = 4
};

enum class dstStationType_enum { // dstStationType
    DS_DEFAULT = 0,
    DS_NO_ROTATE = 1
};

enum class avoidPolicy_enum { // avoidPolicy
    AT_ZERO = 0,
    OBSTACLE_AVOID_WAIT = 1,
    OBSTACLE_AVOID_REPLAN = 2,
    OBSTACLE_AVOID_NONE = 16
};

enum class taskResult_enum { // taskResult
    TASK_RESULT_NA = 0,
    TASK_RESULT_OK = 1,
    TASK_RESULT_CANCELD = 2,
    TASK_RESULT_FAILED = 3
};

struct paths_array_item_struct { //
    pathType_enum pathType; // PathType

    int sx; // sx

    int sy; // sy

    int ex; // ex

    int ey; // ey

    int cx; // cx

    int cy; // cy

    int dx; // dx

    int dy; // dy

    int radius; // radius

    int direction; // direction

    int rotateAngle; // rotateAngle

    int limitV; // limitV

    int limitW; // limitW

    std::string pathName; // 路径名称

    dstStationType_enum dstStationType; // dstStationType

    avoidPolicy_enum avoidPolicy; // avoidPolicy

    taskResult_enum taskResult; // taskResult
};

struct movementState_struct { // 当前移动任务状态 当前移动任务状态
    state_enum state; // TaskState

    int no; // no

    type_enum type; // TaskType

    std::vector<int> stations; // stations

    std::vector<poses_array_item_struct> poses; // poses

    int remainTime; // remainTime

    int remainDistance; // remainDistance

    int totalDistance; // totalDistance

    int curPathNo; // curPathNo

    int curCheckpointNo; // curCheckpointNo

    std::vector<paths_array_item_struct> paths; // paths
};

enum class operationState_enum { //操作状态 操作状态
    OPERATION_NONE = 0,
    OPERATION_AUTO = 1,
    OPERATION_MANUAL = 2
};

enum class fleetMode_enum { //当前的调度模式 当前的调度模式
    FLEET_MODE_NONE = 0,
    FLEET_MODE_OFFLINE = 1,
    FLEET_MODE_ONLINE = 2
};

enum class newMovementTaskState_enum { //标记是否可以开始新移动任务 新移动任务标记
    NEW_MOVEMENT_TASK_STATE_NONE = 0,
    NEW_MOVEMENT_TASK_STATE_READY = 1,
    NEW_MOVEMENT_TASK_STATE_USELESS = 2
};

enum class freshState_enum { //是否是新开启的 新开启状态
    FRESH_NA = 0,
    FRESH_NO = 1,
    FRESH_YES = 2
};

enum class powerState_enum { //电源状态 电源状态
    POWER_NA = 0,
    POWER_NORMAL = 1,
    POWER_SAVE_MODE = 2
};

enum class hstate_enum { //硬件状态 硬件状态
    H_STATE_ZERO = 0,
    H_STATE_INITIALING = 1,
    H_STATE_OK = 2,
    H_STATE_ERROR = 3
};

struct login_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct moveToStation_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct moveToStations_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct moveFollowPath_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct configList_array_item_struct { //
    std::string key; // KEY

    int id; // id

    std::string value; // 名字

    std::string name; // 名字

    std::string valueUnit; // 值单位

    std::string valueType; // 值类型

    std::string defaultValue; // 默认值

    std::string valueRange; // 值范围

    std::string description; // 描述

    std::string permission; // 权限

    std::string changedTime; // 修改时间

    std::string changedUser; // 修改者
};

struct saveConfig_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct commonAction_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct liftActionCmd_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct putDownActionCmd_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct chargeActionCmd_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct stopChargeActionCmd_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct triggerEmergency_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct cancelEmergency_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct pauseMovement_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct continueMovement_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct stopMovement_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct enterPowerSaveMode_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct exitPowerSaveMode_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct startLocation_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct stopLocation_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct enableManualControl_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct disableManualControl_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct setCurMap_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct setCurStation_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct setToCancel_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

struct setupCheckPoint_result_struct { //  响应结果
    int code; // 状态码

    std::string reason; // 原因
};

class standard_oasis_300ul1 {

public:
    // 服务
    login_result_struct login(std::string username, std::string password) // 登录小车 登录小车 sync
    {
        // code
    }
    moveToStation_result_struct moveToStation(int destinationNo) // 移动到站点 移动到站点 sync
    {
        // code
    }
    moveToStations_result_struct moveToStations(std::vector<int> destinationNos) // 移动到多个站点 移动到多个站点 sync
    {
        // code
    }
    moveFollowPath_result_struct moveFollowPath(std::vector<paths_array_item_struct> paths) // 跟随路径移动 跟随路径移动 sync
    {
        // code
    }
    std::vector<configList_array_item_struct> loadConfig() // 加载配置参数 加载配置参数 sync
    {
        // code
    }
    saveConfig_result_struct saveConfig(std::vector<configList_array_item_struct> configList) // 保存配置参数 保存配置参数 sync
    {
        // code
    }
    commonAction_result_struct commonAction(int id, int param0, int param1, std::string str) // 通用动作 通用动作 sync
    {
        // code
    }
    liftActionCmd_result_struct liftActionCmd(int distance) // 顶升活动指令 顶升活动指令 sync
    {
        // code
    }
    putDownActionCmd_result_struct putDownActionCmd() // 放下活动指令 放下活动指令 sync
    {
        // code
    }
    chargeActionCmd_result_struct chargeActionCmd() // 开始充电 开始充电 sync
    {
        // code
    }
    stopChargeActionCmd_result_struct stopChargeActionCmd() // 停止充电 停止充电 sync
    {
        // code
    }
    triggerEmergency_result_struct triggerEmergency() // 触发急停 触发急停 sync
    {
        // code
    }
    cancelEmergency_result_struct cancelEmergency() // 解除急停状态 解除急停状态 sync
    {
        // code
    }
    pauseMovement_result_struct pauseMovement() // 暂停移动 暂停移动 sync
    {
        // code
    }
    continueMovement_result_struct continueMovement() // 继续移动 继续移动 sync
    {
        // code
    }
    stopMovement_result_struct stopMovement() // 停止移动 停止移动 sync
    {
        // code
    }
    enterPowerSaveMode_result_struct enterPowerSaveMode() // 进入低功耗模式 进入低功耗模式 sync
    {
        // code
    }
    exitPowerSaveMode_result_struct exitPowerSaveMode() // 退出低功耗模式 退出低功耗模式 sync
    {
        // code
    }
    startLocation_result_struct startLocation(int stationNo) // 开始定位 开始定位 sync
    {
        // code
    }
    stopLocation_result_struct stopLocation() // 停止定位 停止定位 sync
    {
        // code
    }
    enableManualControl_result_struct enableManualControl() // 启动手动操作 启动手动操作 sync
    {
        // code
    }
    disableManualControl_result_struct disableManualControl() // 停止手动操作 停止手动操作 sync
    {
        // code
    }
    setCurMap_result_struct setCurMap(std::string mapName) // 设置当前地图 设置当前地图 sync
    {
        // code
    }
    setCurStation_result_struct setCurStation(int stationNo) // 设置当前站点 设置当前站点 sync
    {
        // code
    }
    setToCancel_result_struct setToCancel() // 取消任务 取消任务 sync
    {
        // code
    }
    setupCheckPoint_result_struct setupCheckPoint(int curCheckPointNo) // 设置交管点 设置交管点 sync
    {
        // code
    }

public:
    int batteryPercentage; // 电池剩余电量 剩余电量

    batteryState_enum batteryState; // 电池当前状态 电池状态

    emergencyState_enum emergencyState; // 急停状态 急停状态

    breakSwState_enum breakSwState; // 解抱闸状态 解抱闸状态

    sysState_enum sysState; // 系统状态 系统状态

    std::string mapName; // 地图名称 地图名称

    int stationNo; // 站点号 站点号

    locationState_enum locationState; // 定位状态 定位状态

    currentPosition_struct currentPosition; // 当前定位 当前定位

    movementState_struct movementState; // 当前移动任务状态 当前移动任务状态

    operationState_enum operationState; // 操作状态 操作状态

    fleetMode_enum fleetMode; // 当前的调度模式 当前的调度模式

    newMovementTaskState_enum newMovementTaskState; // 标记是否可以开始新移动任务 新移动任务标记

    int multiLoadState; // 多位顶升状态标记 多位顶升状态标记

    int lastErrorCode; // 系统最新错误码 系统最新错误码

    std::vector<int> faultCodesList; // 故障码集 故障码集

    freshState_enum freshState; // 是否是新开启的 新开启状态

    powerState_enum powerState; // 电源状态 电源状态

    hstate_enum hstate; // 硬件状态 硬件状态

    int hardwareErrorCode; // 硬件错误码 硬件错误码

    int boxTemperature; // 机体内部温度，单位℃ 机体内部温度

    int totalMileage; // 总运动里程，单位m 总运动里程

    int totalPoweronTime; // 总开机时间，单位s 总开机时间

    int batteryUseCycles; // 电池循环次数 电池循环次数

    int totalPowerCycle; // 上电次数 上电次数
};
