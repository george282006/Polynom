add_library(polynomial_includes INTERFACE
        polynomial.h
    list.h
        Unordered_tab.h
        AVLtree.h
        HashOpenAddress.h
        RedBlackTree.h
)

target_include_directories(polynomial_includes INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

