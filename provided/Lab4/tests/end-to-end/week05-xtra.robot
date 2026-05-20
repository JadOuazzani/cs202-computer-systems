*** Settings ***
Resource    keyword.resource
Library    ./lib/Errors.py    ${SRC_DIR}/error.h    error_codes    ${SRC_DIR}/error.c    ERR_MESSAGES    prefix=u6fs exited with error:
Library    ./lib/U6fsUtils.py    ${EXE}     ${SRC_DIR}/error.h    error_codes    ${SRC_DIR}/error.c    ERR_MESSAGES    prefix=u6fs exited with error:
Library    ./lib/Utils.py    ${EXE}


*** Variables ***
${DATA_DIR}    ../data


*** Keywords ***
Cat1 template
    [Documentation]  Template for the test of u6fs cat1
    [Arguments]      ${name}    ${inr}
    U6fs Run    ${DATA_DIR}/${name}.uv6    cat1     ${inr}    expected_ret=ERR_NONE    expected_file=${DATA_DIR}/${name}_cat1.txt

Shafiles template
    [Documentation]  Template for the test of u6fs shafiles
    [Arguments]      ${name}
    U6fs Run    ${DATA_DIR}/${name}.uv6    shafiles    expected_ret=ERR_NONE    expected_file=${DATA_DIR}/${name}_shafiles.txt

*** Test Cases ***
Cat1 aiw    [Documentation]    cat1 with aiw.uv6:5 has expected behaviour
    Cat1 template   aiw     5

Shafiles aiw    [Documentation]    shafiles with aiw.uv6 has expected behaviour
    Shafiles template   aiw
