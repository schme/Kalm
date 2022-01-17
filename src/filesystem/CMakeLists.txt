cmake_minimum_required(VERSION 3.18)
project(
	KalmFilesystem
	LANGUAGES CXX)

set(FILESYSTEM_SOURCES
	FileWatcher.cpp
	FileWatcher.h
)

add_library(kalm-filesystem STATIC ${FILESYSTEM_SOURCES})
