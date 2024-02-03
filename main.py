import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns


if __name__ == '__main__':
    df = pd.read_csv('results/dualized_results.csv')

    df2 = pd.read_csv('results/dualized_results_bis.csv')

    df_merge = pd.DataFrame(columns = df.columns)

    for index, row in df.iterrows():
        if row['objective'] != row['lower_bound']:
            row_retested = df2[df2['instance'].str.contains(row['instance'][-22:])]
            row_retested.at[row_retested.index[0], 'instance'] = row['instance']
            df_merge = pd.concat([df_merge, row_retested], ignore_index=True)
        else:
            df_merge = pd.concat([df_merge, row.to_frame().T], ignore_index=True)
    
    df_merge.to_csv('results/dualized_results_merged.csv', index=False)

    


