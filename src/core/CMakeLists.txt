cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(core C)

set(BASE_DIR base)
set(BUS_DIR  bus)
set(DOMAIN_DIR domain)
set(MANAGER_DIR manager)
set(UTIL_DIR util)

INCLUDE_DIRECTORIES(${BASE_DIR} ${BUS_DIR} ${DOMAIN_DIR} ${MANAGER_DIR} ${UTIL_DIR})

set(UX_BASE_HEADER
  ${BASE_DIR}/atomic.h
  ${BASE_DIR}/hash.h
  ${BASE_DIR}/mpscq.h
  ${BASE_DIR}/spscq.h
)

set(UX_BASE_SRC
  ${BASE_DIR}/currency.c
  ${BASE_DIR}/datetime.c
  ${BASE_DIR}/hash.c
  ${BASE_DIR}/memory.c
  ${BASE_DIR}/mpscq.c
  ${BASE_DIR}/spscq.c
)

set(UX_BUS_HEADER
  ${BUS_DIR}/bar_factory.h
  ${BUS_DIR}/context.h
  ${BUS_DIR}/dispatch.h
  ${BUS_DIR}/event_bus.h
  ${BUS_DIR}/event_queue.h
  ${BUS_DIR}/event_vtable.h
)

set(UX_BUS_SRC
  ${BUS_DIR}/bar_factory.c
  ${BUS_DIR}/context.c
  ${BUS_DIR}/dispatch.c
  ${BUS_DIR}/dispatch_execution.c
  ${BUS_DIR}/dispatch_market.c
  ${BUS_DIR}/dispatch_system.c
  ${BUS_DIR}/event.c
  ${BUS_DIR}/event_bus.c
  ${BUS_DIR}/event_queue.c
  ${BUS_DIR}/events.c
)

set(UX_DOMAIN_HEADER
  ${DOMAIN_DIR}/account.h
  ${DOMAIN_DIR}/fill.h
  ${DOMAIN_DIR}/instrument.h
  ${DOMAIN_DIR}/order.h
  ${DOMAIN_DIR}/portfolio.h
  ${DOMAIN_DIR}/position.h
  ${DOMAIN_DIR}/stop.h
  ${DOMAIN_DIR}/strategy.h
  ${DOMAIN_DIR}/subscribe.h
  ${DOMAIN_DIR}/transaction.h
)

set(UX_DOMAIN_SRC
  ${DOMAIN_DIR}/account.c
  ${DOMAIN_DIR}/fill.c
  ${DOMAIN_DIR}/instrument.c
  ${DOMAIN_DIR}/order.c
  ${DOMAIN_DIR}/portfolio.c
  ${DOMAIN_DIR}/position.c
  ${DOMAIN_DIR}/stop.c
  ${DOMAIN_DIR}/strategy.c
  ${DOMAIN_DIR}/subscribe.c
  ${DOMAIN_DIR}/transaction.c
)

set(UX_MANAGER_HEADER
  ${MANAGER_DIR}/data_manager.h
  ${MANAGER_DIR}/instrument_manager.h
  ${MANAGER_DIR}/order_manager.h
  ${MANAGER_DIR}/portfolio_manager.h
  ${MANAGER_DIR}/position_manager.h
  ${MANAGER_DIR}/strategy_manager.h
)

set(UX_MANAGER_SRC
  ${MANAGER_DIR}/data_manager.c
  ${MANAGER_DIR}/instrument_manager.c
  ${MANAGER_DIR}/order_manager.c
  ${MANAGER_DIR}/portfolio_manager.c
  ${MANAGER_DIR}/position_manager.c
  ${MANAGER_DIR}/strategy_manager.c
)

set(UX_UTIL_HEADER
)

set(UX_UTIL_SRC
)

#if(WIN32)
#  list(APPEND UX_UTIL_SRC log_windows.c time_windows.c string_util_windows.c string_windows.c)
#else(WIN32)
#  list(APPEND UX_UTIL_SRC log_posix.c time_posix.c string_posix.c)
#endif()

add_library(uxcore ${LIBUX_BUILD_TYPE} ${UX_BASE_HEADER} ${UX_BASE_SRC} ${UX_BUS_HEADER} ${UX_BUS_SRC} ${UX_DOMAIN_HEADER} ${UX_DOMAIN_SRC}
  ${UX_MANAGER_HEADER} ${UX_MANAGER_SRC} ${UX_UTIL_HEADER} ${UX_UTIL_SRC})
target_link_libraries(uxcore ${UX_EXTERNAL_LIBRARIES})
