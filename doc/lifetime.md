# 生命周期管理
 生命周期管理是C/C++的核心问题
 UX采用引用计数管理ux_event_t和ux_queue_t


## Queue对Event的生命周期管理
   Queue是Event的容器
   * Push/Pop/Peek操作不影响容器内的Event引用计数
   * Destory对容器的Event执行unref操作
