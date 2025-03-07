"""引擎封装包
"""

from . import global_vars
from . import engine
from . import camera
from .util import *


def init():
    """初始化引擎

    @return:
    """
    engine.init_engine()


def start():
    """启动引擎

    @return:
    """
    engine.start_engine()


def quit():
    """退出引擎

    @return:
    """
    engine.quit_engine()


def get_screen():
    """获得屏幕引用

    @return:
    """
    return global_vars.screen


def get_scene():
    """获得场景引用

    @return:
    """
    return global_vars.scene


def set_speed(speed: float=1.0):
    """设置游戏速率

    @param speed:
    @return:
    """
    global_vars.speed_times = speed


def get_speed() -> float:
    """返回游戏速度

    @return:
    """
    return global_vars.speed_times


def get_camera_mgr() -> camera.CameraMgr:
    """获得摄像机管理器

    @return:
    """
    return global_vars.camera_mgr


def get_main_camera() -> camera.Camera:
    """返回主摄像机

    @return:
    """
    return get_camera_mgr().get_main_camera()


def register_handle(event_type, handle_func):
    """注册事件处理函数

    @param event_type: 事件类型
    @param handle_func: 处理函数
    @return:
    """
    global_vars.event_mgr.register(event_type, handle_func)


def get_real_run_time() -> float:
    """获得真实运行时间

    @return:
    """
    return global_vars.real_run_time


def get_game_run_time() -> float:
    """获得游戏运行时间

    @return:
    """
    return global_vars.game_run_time
