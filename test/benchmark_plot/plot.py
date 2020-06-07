import matplotlib.pyplot as plt
import numpy as np
import sys

upper_bound = int(sys.argv[1])
repeat = list(range(1, upper_bound + 1))

real, user, sys = np.loadtxt('test/benchmark_plot/time_result.txt', delimiter=',', unpack=True)

real_avg = sum(real) / len(real)
user_avg = sum(user) / len(user)
sys_avg = sum(sys) / len(sys)

real_avg_list = [real_avg] * upper_bound
user_avg_list = [user_avg] * upper_bound
sys_avg_list = [sys_avg] * upper_bound

# plot actual
plt.plot(repeat, real, label='real')
plt.plot(repeat, user, label='user', color="violet")
plt.plot(repeat, sys, label='sys', color="cyan")

# plot average
plt.plot(repeat, real_avg_list, label='real average')
plt.plot(repeat, user_avg_list, label='user average')
plt.plot(repeat, sys_avg_list, label='sys average')

plt.xlabel("repetitions")
plt.ylabel("used time(seconds)")
plt.title("benchmark of vector push function")

leg = plt.legend()
# get the lines inside legend box
leg_lines = leg.get_lines()
# bulk-set the properties of all lines
plt.setp(leg_lines, linewidth=2)
plt.savefig('test/benchmark_plot/result')
plt.show()

