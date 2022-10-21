message(${CMAKE_CURRENT_SOURCE_DIR})

macro(init_path)
    message("init library path")
    # 设置 SLD 安装路径
    set(sdl_dir "/usr/local/Cellar/sdl2/2.0.22")
    # 设置 SLD_IMAGE 安装路径
    set(sdl_image_dir "/usr/local/Cellar/sdl2_image/2.6.0")
endmacro()

macro(link_sdl)
    message("link sdl library")
    # 设置要包含头文件的路径
    include_directories(${sdl_dir}/include/SDL2)
    # 设置要关联库的路径
    link_directories(${sdl_dir}/lib)
endmacro()

macro(link_sdl_image)
    message("link sdl image library")
    # 设置要包含头文件的路径
    include_directories(${sdl_image_dir}/include/SDL2)
    # 设置要关联库的路径
    link_directories(${sdl_image_dir}/lib)
endmacro()


macro(target_link_sdl name)
    message("${name} link sdl image")
    target_link_libraries(${name}
            SDL2
            SDL2_test
            SDL2main
            )
endmacro()


macro(target_link_sdl_image name)
    message("${name} link sdl image")
    # 把目标执行文件与 SDL_image 开源库关联
    target_link_libraries(${name}
            SDL2_image
            )
endmacro()



macro(init_definition)
    add_definitions(-DPROJECT_DIR_PATH="${CMAKE_SOURCE_DIR}")
endmacro()
