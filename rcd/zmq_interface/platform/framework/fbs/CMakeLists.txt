cmake_project(FBS)

execute_process(COMMAND
                sh -c "${FBS_SOURCE_DIR}/auto_gen.sh ${FBS_SOURCE_DIR}")