cmake_minimum_required(VERSION 3.16)

project("noise")

add_library("noise" INTERFACE)

target_include_directories("noise" INTERFACE include "include")