//
// Created by Shiro on 2023/8/26.
//

#include "BasicLog.h"
#include <format>
#include <iostream>

std::ofstream LoggerOutput::log_file_out("log.txt", std::ios::out);