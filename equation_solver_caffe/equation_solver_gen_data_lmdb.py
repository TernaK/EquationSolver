import cv2
import numpy as np
import lmdb
import caffe

N = 13*13*4

# Let's pretend this is interesting data
#X = np.zeros((N, 1, 28, 28), dtype=np.uint8)
#y = np.zeros(N, dtype=np.int64)

# We need to prepare the database for the size. We'll set it 10 times
# greater than what we theoretically need. There is little drawback to
# setting this too big. If you still run into problem after raising
# this, you might want to try saving fewer entries in a single
# transaction.
map_size = 51000*28*28*5#X.nbytes * 10

env = lmdb.open('equation_solver_lmdb', map_size=map_size)

index = 0

with env.begin(write=True) as txn:
    # first do operands 
    path1 = 'operators/'
    path2 = ['plus/', 'minus/', 'divide/', 'multiply/']
    labels = [10,11,12,13]

    for p in range(4):
        for i in range(N/4):
            X = cv2.imread(path1+path2[p]+str(i)+'.jpg', cv2.IMREAD_GRAYSCALE)
            datum = caffe.proto.caffe_pb2.Datum()
            datum.channels = 1
            datum.height = 28
            datum.width = 28
            datum.data = X.tostring()  # or .tostring() if numpy < 1.9
            datum.label = labels[p]
            str_id = '{:08}'.format(index)
            index = index + 1

            txn.put(str_id.encode('ascii'), datum.SerializeToString())

    # second do mnist
    lmdb_env = lmdb.open('/home/pi/caffe/examples/mnist/mnist_train_lmdb')
    lmdb_txn = lmdb_env.begin()
    lmdb_cursor = lmdb_txn.cursor()
    datum = caffe.proto.caffe_pb2.Datum()

    for key, value in lmdb_cursor:
        datum.ParseFromString(value)
        str_id = '{:08}'.format(index)
        index = index + 1

        txn.put(str_id.encode('ascii'), datum.SerializeToString())
