import desbordante

TABLE = 'examples/datasets/inventory_afd.csv'
ERROR = 0.3
ERROR_MEASURE = 'per_value' # per_tuple or per_value

algo = desbordante.PFDTane()
algo.load_data(TABLE, ',', True)
algo.set_option('error_measure', ERROR_MEASURE)
algo.execute(error=ERROR)
result = algo.get_fds()
print('pFDs:')
for fd in result:
    print(fd)
