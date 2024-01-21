import desbordante

TABLE = 'examples/datasets/inventory_afd.csv'
ERROR = 0.2
ERROR_MEASURE = 'per_value' # per_tuple or per_value


def make_set(fds):
    return set(map(str, fds))


def get_afds():
    algo = desbordante.Tane()
    algo.load_data(TABLE, ',', True)
    algo.execute(error=ERROR)
    return algo.get_fds()


def get_pfds():
    algo = desbordante.PFDTane()
    algo.load_data(TABLE, ',', True)
    algo.set_option('error_measure', ERROR_MEASURE)
    algo.execute(error=ERROR)
    return algo.get_fds()


pfds = make_set(get_pfds())
afds = make_set(get_afds())

print("pFDs \ AFDs =", pfds - afds)
print("AFDs \ pFDs =", afds - pfds)
print("AFDs ∩ pFDs =", afds & pfds)
