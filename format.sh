set -f
    find ./src/ \( -name '*.cpp' \) -exec clang-format -i {} \;
    find ./include/ \( -name '*.hpp' \) -exec clang-format -i {} \;
set +f