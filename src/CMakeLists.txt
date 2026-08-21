add_executable(SoftwareRasterizer)

file(GLOB  SOURCES  CONFIGURE_DEPENDS  "*.cpp")
file(GLOB  HEADERS  CONFIGURE_DEPENDS  "*.hpp")

target_sources(SoftwareRasterizer
  PRIVATE
    ${SOURCES}
    ${HEADERS}
)