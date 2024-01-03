from prettytable import PrettyTable
import matplotlib.pyplot as plt

def create_table():
    table = PrettyTable()
    table.field_names = ["Step", "Page", "Memory State", "Action"]
    table.align["Memory State"] = "l"
    table.align["Action"] = "r"
    return table

def optimal_page_replacement(pages, frames):
    memory = []
    page_faults = 0
    table = create_table()

    for i, page in enumerate(pages):
        action = ''
        if page not in memory:
            if len(memory) < frames:
                memory.append(page)
                action = f'push({page})'
            else:
                future = pages[i + 1:]
                farthest = max(memory, key=lambda m: future.index(m) if m in future else float('inf'))
                memory.remove(farthest)
                memory.append(page)
                action = f'pop({farthest}), push({page})'
            page_faults += 1
        table.add_row([i + 1, page, ' '.join(str(m) for m in memory), action])

    print("Optimal Page Replacement:")
    print(table)
    print(f"Total page faults: {page_faults}\n")

    return page_faults

def lru_page_replacement(pages, frames):
    memory, recent, page_faults = [], [], 0
    table = create_table()

    for i, page in enumerate(pages):
        action = ''
        if page not in memory:
            if len(memory) < frames:
                memory.append(page)
                recent.append(page)
                action = f'push({page})'
            else:
                lru = recent.pop(0)
                memory.remove(lru)
                memory.append(page)
                recent.append(page)
                action = f'pop({lru}), push({page})'
            page_faults += 1
        else:
            recent.remove(page)
            recent.append(page)

        if action:
            table.add_row([i + 1, page, ' '.join(str(m) for m in memory), action])

    print("Least Recently Used (LRU) Page Replacement:")
    print(table)
    print(f"Total page faults: {page_faults}\n")

    return page_faults

def fifo_page_replacement(pages, frames):
    memory, page_faults = [], 0
    table = create_table()

    for i, page in enumerate(pages):
        action = ''
        if page not in memory:
            if len(memory) < frames:
                memory.append(page)
                action = f'push({page})'
            else:
                removed = memory.pop(0)
                memory.append(page)
                action = f'pop({removed}), push({page})'
            page_faults += 1
        table.add_row([i + 1, page, ' '.join(str(m) for m in memory), action])

    print("First-In, First-Out (FIFO) Page Replacement:")
    print(table)
    print(f"Total page faults: {page_faults}\n")

    return page_faults

# pages = [22, 5, 10, 22, 5, 4, 2, 17, 20, 19, 6, 14, 17, 19, 6, 14, 6, 21, 2, 18, 4, 21, 4, 2, 8, 2, 22, 2, 20, 13, 10, 1, 10, 5, 12, 11, 1, 21]
# frames = 6

# optimal_page_replacement(pages, frames)
# lru_page_replacement(pages, frames)
# fifo_page_replacement(pages, frames)


def compare_algorithms(pages, frame_sizes):
    table = PrettyTable()
    table.field_names = ["Algorithm", "Frames", "Page Faults"]

    for frames in frame_sizes:
        table.add_row(["Optimal", frames, optimal_page_replacement(pages, frames)])
        table.add_row(["LRU", frames, lru_page_replacement(pages, frames)])
        table.add_row(["FIFO", frames, fifo_page_replacement(pages, frames)])

    print(table)

def plot_page_faults(pages, frame_range):
    optimal_faults = [optimal_page_replacement(pages, frames) for frames in frame_range]
    lru_faults = [lru_page_replacement(pages, frames) for frames in frame_range]
    fifo_faults = [fifo_page_replacement(pages, frames) for frames in frame_range]

    plt.plot(frame_range, optimal_faults, marker='o')
    plt.plot(frame_range, lru_faults, marker='s')
    plt.plot(frame_range, fifo_faults, marker='^')

    plt.text(frame_range[0], optimal_faults[0], 'Optimal', fontsize=9, verticalalignment='bottom')
    plt.text(frame_range[0], lru_faults[0], 'LRU', fontsize=9, verticalalignment='bottom')
    plt.text(frame_range[0], fifo_faults[0], 'FIFO', fontsize=9, verticalalignment='bottom')

    plt.xlabel('Number of Frames')
    plt.ylabel('Page Faults')
    plt.title('Page Replacement Algorithms Comparison')
    plt.grid(True)

    plt.show()


pages = [22, 5, 10, 22, 5, 4, 2, 17, 20, 19, 6, 14, 17, 19, 6, 14, 6, 21, 2, 18, 4, 21, 4, 2, 8, 2, 22, 2, 20, 13, 10, 1, 10, 5, 12, 11, 1, 21]
frame_range = range(2, 13)

compare_algorithms(pages, [3, 6, 12])

plot_page_faults(pages, frame_range)