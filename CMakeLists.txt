cmake_minimum_required(VERSION 3.12)
project(Coursework_CPP)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Coursework_CPP
        bunny/data/bun.conf
        bunny/data/bun000.ply
        bunny/data/bun045.ply
        bunny/data/bun090.ply
        bunny/data/bun180.ply
        bunny/data/bun270.ply
        bunny/data/bun315.ply
        bunny/data/chin.ply
        bunny/data/ear_back.ply
        bunny/data/README
        bunny/data/top2.ply
        bunny/data/top3.ply
        bunny/reconstruction/bun_zipper.ply
        bunny/reconstruction/bun_zipper_res2.ply
        bunny/reconstruction/bun_zipper_res3.ply
        bunny/reconstruction/bun_zipper_res4.ply
        bunny/reconstruction/README
        bunny_zipper.kcply
        colour.h
        directional_light.cpp
        directional_light.h
        hit.h
        image_1024.ppm
        image_64.ppm
        light.h
        main.cpp
        material.h
        object.h
        phong.cpp
        phong.h
        polymesh.cpp
        polymesh.h
        ray.h
        scene.cpp
        scene.h
        sphere.cpp
        sphere.h
        transform.h
        vector.h
        vertex.h)
