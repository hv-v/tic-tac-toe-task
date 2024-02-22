cmake_minimum_required(VERSION 3.16)

project(tic-tac-toe VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Core Qml)

qt_standard_project_setup()

set(QML_SOURCES
	Main.qml
	TicTacToeField.qml
	TicTacToeGame.qml
	TicTacToeGameConfig.qml
	TicTacToePlayerDelegate.qml
	TicTacToeGameButtons.qml
)

set(APP_SOURCES
	app/Player.h
	app/Player.cpp
	app/HumanPlayerImpl.cpp
	app/BotPlayerImpl.cpp
	app/GameBoardImpl.h
	app/GameBoardImpl.cpp
	app/GameImpl.h
	app/GameImpl.cpp
)

set(UTILS_SOURCES
	utils/Utils.h
)

set(MODELS_SOURCES
	models/GameBoardModel.h
	models/GameBoardModel.cpp
	models/GameModel.h
	models/GameModel.cpp
	models/PlayerConfigModel.h
	models/PlayerConfigModel.cpp
)

qt_add_executable(app-tic-tac-toe
	main.cpp
	helpers.h

	${APP_SOURCES}
	${MODELS_SOURCES}
	${UTILS_SOURCES}
)

qt_add_qml_module(app-tic-tac-toe
	URI tic-tac-toe
	VERSION 1.0
	QML_FILES ${QML_SOURCES}
)

set_target_properties(app-tic-tac-toe PROPERTIES
	MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

target_link_libraries(app-tic-tac-toe
	PRIVATE Qt6::Quick Qt6::Core Qt6::Qml
)

target_include_directories(app-tic-tac-toe PRIVATE ${CMAKE_SOURCE_DIR})

install(TARGETS app-tic-tac-toe
	BUNDLE DESTINATION .
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
