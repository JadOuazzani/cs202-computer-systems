*** Variables ***
${DATA_DIR}    ../data

*** Settings ***
Resource    keyword.resource
Library    ./lib/Errors.py    ${SRC_DIR}/error.h    error_codes    ${SRC_DIR}/error.c    ERR_MESSAGES    prefix=u6fs exited with error:
Library    ./lib/U6fsUtils.py    ${EXE}     ${SRC_DIR}/error.h    error_codes    ${SRC_DIR}/error.c    ERR_MESSAGES    prefix=u6fs exited with error:
Library    ./lib/Utils.py    ${EXE}

*** Keywords ***
Tree template
    [Documentation]  Template for the test of u6fs tree
    [Arguments]      ${name}
    U6fs Run    ${DATA_DIR}/${name}.uv6    tree     expected_ret=ERR_NONE    expected_file=${DATA_DIR}/${name}_tree.txt

*** Test Cases ***    
Tree aiw    [Documentation]    tree with aiw.uv6 has expected behaviour
    Tree template   aiw
