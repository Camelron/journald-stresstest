import random
import string
import time
import logging
from systemd.journal import JournalHandler
import uuid

def main():
    print("Starting to log to journald...")
    log = logging.getLogger(__name__)
    log.propagate = False
    log.addHandler(JournalHandler())
    
    while True:
        rval = gen_rand()
        print(rval)
        log.warning('Logging random string: %s', rval, extra={'MESSAGE_ID': str(uuid.uuid4())})
        time.sleep(5)

def gen_rand(chars = string.ascii_uppercase + string.digits, N=16):
    return ''.join(random.choice(chars) for _ in range(N))

if __name__ == "__main__":
    main()
