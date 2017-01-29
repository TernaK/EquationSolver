import cv2
import numpy as np
import lmdb
import caffe
# Let's pretend this is interesting data
#X = np.zeros((N, 1, 28, 28), dtype=np.uint8)
#y = np.zeros(N, dtype=np.int64)

# We need to prepare the database for the size. We'll set it 10 times
# greater than what we theoretically need. There is little drawback to
# setting this too big. If you still run into problem after raising
# this, you might want to try saving fewer entries in a single
# transaction.
def readImageWriteLmdb(readPath, label, index):
    with env.begin(write=True) as txn:
        #print(readPath)
        X = cv2.imread(readPath, cv2.IMREAD_GRAYSCALE)
        datum = caffe.proto.caffe_pb2.Datum()
        datum.channels = 1
        datum.height = 28
        datum.width = 28
        datum.data = X.tostring()  # or .tostring() if numpy < 1.9
        datum.label = label
        str_id = '{:08}'.format(index)
        index = index + 1
        txn.put(str_id.encode('ascii'), datum.SerializeToString())
        return

N = 13*13*14

map_size = N*10*28*28

env = lmdb.open('equation_solver_lmdb', map_size=map_size)

index = 0

#get operators
path1 = 'operators/'
path2 = ['plus/', 'minus/', 'divide/', 'multiply/']
labels = [10,11,12,13]

for p in range(4):
    for i in range(N/14):
        readImageWriteLmdb(path1+path2[p]+str(i)+'.png', labels[p], index)
        index += 1
        
#get operands
path1 = 'operands/'
for p in range(10):
    for i in range(N/14):
        readImageWriteLmdb(path1+str(p)+'/'+str(i)+'.png', p, index)
        index += 1
