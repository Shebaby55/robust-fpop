import numpy as np
import os
import pickle

# Initialize the Simu list
Simu = []

########## SIMU 0
isimu = 0
Simu.append({})
Simu[isimu]['bkpPage29'] = np.array([0] + [x - 1 for x in [205, 267, 308, 472, 512, 820, 902, 1332, 1557, 1598, 1659]] + [2048])
Simu[isimu]['Lg'] = np.diff(Simu[isimu]['bkpPage29'])
Simu[isimu]['mu'] = np.array([0, 14.64, -3.66, 7.32, -7.32, 10.98, -4.39, 3.29, 19.03, 7.68, 15.37, 0])
Simu[isimu]['sigma'] = 10
Simu[isimu]['signal'] = np.repeat(Simu[isimu]['mu'], Simu[isimu]['Lg'])
Simu[isimu]['Ktrue'] = np.sum(np.diff(Simu[isimu]['signal']) != 0) + 1

########## SIMU 1
isimu = 1
Simu.append({})
Simu[isimu]['bkpPage29'] = np.array([0] + [x - 1 for x in [139, 226, 243, 300, 309, 333]]+ [497])
Simu[isimu]['Lg'] = np.diff(Simu[isimu]['bkpPage29'])
Simu[isimu]['mu'] = np.array([-0.18, 0.08, 1.07, -0.53, 0.16, -0.69, -0.16])
Simu[isimu]['sigma'] = 0.3
Simu[isimu]['signal'] = np.repeat(Simu[isimu]['mu'], Simu[isimu]['Lg'])
Simu[isimu]['Ktrue'] = np.sum(np.diff(Simu[isimu]['signal']) != 0) + 1

########## SIMU 2'
isimu = 2
Simu.append({})
Simu[isimu]['bkpPage29'] = np.array([0] + [x - 1 for x in [139, 226, 243, 300, 309, 333]]+ [497])
Simu[isimu]['Lg'] = np.diff(Simu[isimu]['bkpPage29'])
Simu[isimu]['mu'] = np.array([-0.18, 0.08, 1.07, -0.53, 0.16, -0.69, -0.16])
Simu[isimu]['sigma'] = np.sqrt(0.04)
Simu[isimu]['signal'] = np.repeat(Simu[isimu]['mu'], Simu[isimu]['Lg'])
Simu[isimu]['Ktrue'] = np.sum(np.diff(Simu[isimu]['signal']) != 0) + 1

########## SIMU 3
isimu = 3
Simu.append({})
Simu[isimu]['bkpPage29'] = np.array([0] + [x - 1 for x in [11, 21, 41, 61, 91, 121, 161, 201, 251, 301, 361, 421, 491]] + [560])
Simu[isimu]['Lg'] = np.diff(Simu[isimu]['bkpPage29'])
Simu[isimu]['mu'] = np.array([7, -7, 6, -6, 5, -5, 4, -4, 3, -3, 2, -2, 1, -1])
Simu[isimu]['sigma'] = 4
Simu[isimu]['signal'] = np.repeat(Simu[isimu]['mu'], Simu[isimu]['Lg'])
Simu[isimu]['Ktrue'] = np.sum(np.diff(Simu[isimu]['signal']) != 0) + 1

########## SIMU 4
isimu = 4
Simu.append({})
Simu[isimu]['bkpPage29'] = np.array([0] + [x - 1 for x in [11, 21, 31, 41, 51, 61, 71, 81, 91, 101, 111, 121, 131]] + [140])
Simu[isimu]['Lg'] = np.diff(Simu[isimu]['bkpPage29'])
Simu[isimu]['mu'] = np.array([0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1])
Simu[isimu]['sigma'] = 0.4
Simu[isimu]['signal'] = np.repeat(Simu[isimu]['mu'], Simu[isimu]['Lg'])
Simu[isimu]['Ktrue'] = np.sum(np.diff(Simu[isimu]['signal']) != 0) + 1

########## SIMU 5
isimu = 5
Simu.append({})
Simu[isimu]['bkpPage29'] = np.array([0] + [x - 1 for x in [11, 21, 31, 41, 51, 61, 71, 81, 91, 101, 111, 121, 131, 141]]+ [150])
Simu[isimu]['Lg'] = np.diff(Simu[isimu]['bkpPage29'])
Simu[isimu]['mu'] = np.array(list(range(1, 16)))
Simu[isimu]['sigma'] = 0.3
Simu[isimu]['signal'] = np.repeat(Simu[isimu]['mu'], Simu[isimu]['Lg'])
Simu[isimu]['Ktrue'] = np.sum(np.diff(Simu[isimu]['signal']) != 0) + 1



with open(os.path.join(os.getcwd(), 'py', 'Simu.pkl'), 'wb') as f:
    pickle.dump(Simu, f)

# Example usage
if __name__ == "__main__":
    print("Simulation Data:")
    for i, sim in enumerate(Simu, start=1):
        print(f"SIMU {i}:")
        print(f"  bkpPage29: {sim['bkpPage29']}")
        print(f"  Lg: {sim['Lg']}")
        print(f"  mu: {sim['mu']}")
        print(f"  sigma: {sim['sigma']}")
        print(f"  signal: {sim['signal'][:10]} ... {sim['signal'][-10:]}")
        print(f"  Ktrue: {sim['Ktrue']}")
        print()