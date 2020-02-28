message(STATUS "Compile core")

# file(GLOB SRC_LIST "*.cc" )
# add_library(core STATIC ${SRC_LIST})
# set(__smcc_lib "${__smcc_lib};core" CACHE INTERNAL "")

smcc_library(asd ast.cc)
smcc_library(asd_v2 ast_v2.cc)
smcc_library(codegen codegen.cc)
smcc_library(node node.cc)
smcc_library(token token.cc)
