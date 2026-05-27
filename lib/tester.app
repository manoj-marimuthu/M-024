fn log_success(test_name):
    log_msg = "[OK] " + test_name
    print(log_msg)

fn log_failure(test_name,expected,received):
    print("[FAILED] ",test_name,"| Expected:",expected,", Received:",received)

fn testEqual(a,b,test_name):
    if a == b:
        log_success(test_name)
    else:
        log_failure(test_name,a,b)

fn testNotEqual(a,b,test_name):
    if a != b:
        log_success(test_name)
    else:
        log_failure(test_name,a,b)

fn testTrue(a,test_name):
    if a:
        log_success(test_name)
    else:
        log_failure(test_name,a,True)

fn testFalse(a,test_name):
    if not a:
        log_success(test_name)
    else:
        log_failure(test_name,a,False)