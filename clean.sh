ps -ef | grep db_server  | awk '{print $2}' | xargs kill -9
