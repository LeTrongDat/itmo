from tabulate import tabulate

class Process:
    def __init__(self, pid, bursts, arrival_time):
        self.pid = pid
        self.bursts = bursts
        self.arrival_time = arrival_time
        self.current_burst_index = 0  
        self.current_burst_time = 0
        self.status = 'new'
        self.completion_time = None
        self.waiting_time = 0
        self.current_burst_index = 0
        self.completed = False
        self.service_time = sum(time for _, time in bursts)
        self.start_time = None  
        self.first_scheduled = False


    def mark_as_complete(self, completion_time):
        self.completed = True
        self.completion_time = completion_time

    def is_done(self):
        return self.completed

    def update_status(self, new_status):
        self.status = new_status

    def is_cpu_burst_complete(self):
        if self.current_burst_index < len(self.bursts) and self.bursts[self.current_burst_index][1] <= self.current_burst_time:
            return True
        return False

    def is_io_burst_complete(self):
        if self.current_burst_index < len(self.bursts) and self.bursts[self.current_burst_index][1] <= self.current_burst_time:
            return True
        return False

    def increment_burst_time(self):
        self.current_burst_time += 1

    def has_next_io_burst(self):
        return self.next_io_burst_id()

    def next_io_burst_id(self):
        for index in range(self.current_burst_index, len(self.bursts)):
            burst_type, _ = self.bursts[index]
            if burst_type.startswith('IO'):
                return burst_type 
        return None

    def has_next_cpu_burst(self):
        return self.next_cpu_burst_id()

    def next_cpu_burst_id(self):
        for index in range(self.current_burst_index, len(self.bursts)):
            burst_type, _ = self.bursts[index]
            if burst_type.startswith('CPU'):
                return burst_type 
        return None

    def move_to_next_burst(self):
        self.current_burst_index += 1
        self.current_burst_time = 0

    def increment_waiting_time(self):
        self.waiting_time += 1

    def set_start_time(self, current_time):
        if not self.first_scheduled:
            self.start_time = current_time
            self.first_scheduled = True

    def remaining_cpu_burst_time(self):
        if self.current_burst_index < len(self.bursts):
            burst_type, burst_duration = self.bursts[self.current_burst_index]
            if burst_type == 'CPU':
                return burst_duration
        return float('inf')
    
    def response_ratio(self):
        if self.service_time > 0:
            return (self.waiting_time + self.service_time) / self.service_time
        return float('inf')

class OS:
    def __init__(self, num_processors, num_io_devices, scheduling_algorithm="fcfs", time_quantum=None):
        self.processors = [Processor() for _ in range(num_processors)]
        self.io_devices = [IODevice(f'IO{i+1}') for i in range(num_io_devices)]
        self.ready_queue = []
        self.all_processes = []
        self.current_time = 0
        self.scheduling_algorithm = scheduling_algorithm
        self.time_quantum = time_quantum
        self.quantum_counter = 0

    def add_process(self, process):
        self.all_processes.append(process)

    def run(self):
        while any([process for process in self.all_processes if not process.is_done()]):
            self.update_queues()

            self.schedule()

            for processor in self.processors:
                if not processor.is_idle():
                    processor.process()

            self.process_io()
            
            for io_device in self.io_devices:
                if not io_device.is_idle():
                    io_device.process()

            self.increment_waiting_times()

            self.current_time += 1

        # print(f"Simulation completed at time {self.current_time} ms")

    def update_queues(self):
        for process in self.all_processes:
            if process.arrival_time <= self.current_time and process.status == 'new':
                self.ready_queue.append(process)
                process.update_status('running')

        for processor in self.processors:
            if processor.is_idle():
                continue
            current_process = processor.current_process
            if current_process and current_process.is_cpu_burst_complete():
                current_process.move_to_next_burst()
                processor.remove_current_process()
                if current_process.has_next_io_burst():
                    io_device = self.select_io_device_for_process(current_process)
                    io_device.add_process(current_process)
                else:
                    current_process.mark_as_complete(self.current_time)
                continue

            if self.scheduling_algorithm == "round robin" and current_process.current_burst_time % self.time_quantum == 0:
                processor.remove_current_process()
                self.ready_queue.append(current_process)
                continue

            if self.scheduling_algorithm == "srt":
                self.ready_queue.append(current_process)
                processor.remove_current_process()
                continue

        for io_device in self.io_devices:
            if io_device.is_idle():
                continue
            current_process = io_device.current_process
            if current_process and current_process.is_io_burst_complete():
                current_process.move_to_next_burst()
                io_device.remove_current_process()
                if current_process.has_next_cpu_burst():
                    self.ready_queue.append(current_process)
                else:
                    current_process.mark_as_complete(self.current_time)

    def schedule(self):
        if self.scheduling_algorithm == "spn":
            self.schedule_spn()
        elif self.scheduling_algorithm == "round robin":
            self.schedule_round_robin()
        elif self.scheduling_algorithm == "srt":
            self.schedule_srt()
        elif self.scheduling_algorithm == "hrrn":
            self.schedule_hrrn()
        else:
            self.schedule_fcfs()

    def schedule_hrrn(self):
        self.ready_queue.sort(key=lambda p: p.response_ratio(), reverse=True)

        for processor in self.processors:
            if processor.is_idle() and self.ready_queue:
                next_process = self.ready_queue.pop(0)
                if next_process.start_time is None:
                    next_process.set_start_time(self.current_time)
                processor.assign_process(next_process)

    def schedule_srt(self):
        self.ready_queue.sort(key=lambda p: (p.remaining_cpu_burst_time(), p.pid))
        for processor in self.processors:
            if processor.is_idle() and self.ready_queue:
                next_process = self.ready_queue.pop(0)
                if next_process.start_time is None:
                    next_process.set_start_time(self.current_time)
                processor.assign_process(next_process)

    def schedule_spn(self):
        self.ready_queue.sort(key=lambda p: p.remaining_cpu_burst_time())
        
        for processor in self.processors:
            if processor.is_idle() and self.ready_queue:
                next_process = self.ready_queue.pop(0)
                if next_process.start_time is None:
                    next_process.set_start_time(self.current_time)
                processor.assign_process(next_process)

    def schedule_round_robin(self):
        self.schedule_fcfs()

    def schedule_fcfs(self):
        for processor in self.processors:
            if processor.is_idle() and self.ready_queue:
                next_process = self.ready_queue.pop(0)
                if next_process.start_time is None:
                    next_process.set_start_time(self.current_time)
                processor.assign_process(next_process)


    def process_io(self):
        for io_device in self.io_devices:
            if io_device.is_idle() and io_device.has_waiting_process():
                next_process = io_device.get_next_process()
                io_device.assign_process(next_process)

    def increment_waiting_times(self):
        for process in self.ready_queue:
            process.increment_waiting_time()

        for io_device in self.io_devices:
            for process in io_device.queue:
                process.increment_waiting_time()

    def select_io_device_for_process(self, process):
        if process.has_next_io_burst():
            io_burst_id = process.next_io_burst_id()

            for device in self.io_devices:
                if device.id == io_burst_id:
                    return device


class IODevice:
    def __init__(self, device_id):
        self.id = device_id
        self.queue = []
        self.current_process = None

    def add_process(self, process):
        self.queue.append(process)

    def is_idle(self):
        return self.current_process is None

    def get_next_process(self):
        if self.queue:
            return self.queue.pop(0)
        return None

    def assign_process(self, process):
        self.current_process = process

    def has_waiting_process(self):
        return len(self.queue) > 0

    def process(self):
        if self.current_process:
            self.current_process.increment_burst_time()
            if self.current_process.is_io_burst_complete():
                # TO-DO: Handle completion of the burst
                pass

    def remove_current_process(self):
        self.current_process = None

class Processor:
    def __init__(self):
        self.current_process = None

    def assign_process(self, process):
        self.current_process = process

    def is_idle(self):
        return self.current_process is None

    def process(self):
        if self.current_process:
            self.current_process.increment_burst_time()
            if self.current_process.is_cpu_burst_complete():
                # TO-DO: Handle completion of the burst
                pass

    def remove_current_process(self):
        self.current_process = None

def collect_statistics(my_os):
    stats = {'completed time': my_os.current_time}
    for process in my_os.all_processes:
        turnaround_time = process.waiting_time + process.service_time
        tr_ts_ratio = turnaround_time / process.service_time if process.service_time > 0 else 0
        stats[process.pid] = {
            'arrival': process.arrival_time,
            'service': process.service_time,
            'start': process.start_time,
            'finish': process.completion_time,
            'turnaround': turnaround_time,
            'tr/ts': round(tr_ts_ratio, 2),
        }
    return stats


def simulate(scheduling_algorithm, time_quantum=None):
    my_os = OS(num_processors=4, num_io_devices=2, scheduling_algorithm=scheduling_algorithm, time_quantum=time_quantum)

    processes = [
        Process(1, [('CPU', 48), ('IO1', 12), ('CPU', 36), ('IO2', 16), ('CPU', 12), ('IO1', 12), ('CPU', 24), ('IO1', 14), ('CPU', 48), ('IO1', 12)], arrival_time=0),
        Process(2, [('CPU', 60), ('IO1', 10), ('CPU', 60), ('IO1', 18), ('CPU', 36), ('IO1', 14), ('CPU', 12), ('IO2', 20), ('CPU', 12), ('IO1', 16), ('CPU', 48), ('IO1', 20), ('CPU', 36), ('IO1', 20)], arrival_time=2),
        Process(3, [('CPU', 2), ('IO2', 10), ('CPU', 8), ('IO2', 10), ('CPU', 2), ('IO1', 12), ('CPU', 6), ('IO2', 10), ('CPU', 6), ('IO1', 18), ('CPU', 10), ('IO2', 20)], arrival_time=4),
        Process(4, [('CPU', 24), ('IO2', 20), ('CPU', 48), ('IO2', 14), ('CPU', 36), ('IO1', 10), ('CPU', 24), ('IO1', 18)], arrival_time=6),
        Process(5, [('CPU', 48), ('IO2', 20), ('CPU', 24), ('IO1', 12), ('CPU', 36), ('IO1', 20), ('CPU', 24), ('IO1', 14), ('CPU', 36), ('IO1', 20)], arrival_time=8),
        Process(6, [('CPU', 2), ('IO1', 20), ('CPU', 4), ('IO1', 20), ('CPU', 2), ('IO2', 12), ('CPU', 10), ('IO2', 20), ('CPU', 8), ('IO1', 14)], arrival_time=10)
    ]

    # processes = [
    #     Process(1, [('CPU', 3)], 0),
    #     Process(2, [('CPU', 6)], 2),
    #     Process(3, [('CPU', 4)], 4),
    #     Process(4, [('CPU', 5)], 6),
    #     Process(5, [('CPU', 2)], 8), 
    # ]

    for process in processes:
        my_os.add_process(process)

    my_os.run()

    return collect_statistics(my_os)

def display_results(results):
    headers = ["PID"] + [f"Process {pid}" for pid in range(1, 7)]
    
    for algorithm, stats in results.items():
        print(f"\n{algorithm} Scheduling Statistics: {stats['completed time']} ms")
        table_data = []
        for stat_name in ['arrival', 'service', 'start', 'finish', 'turnaround', 'tr/ts']:
            row = [stat_name] + [stats[pid][stat_name] for pid in range(1, 6)]
            table_data.append(row)
        
        print(tabulate(table_data, headers=headers, tablefmt="grid"))

def main():
    results = {}
    results['FCFS'] = simulate(None)
    results['RR-1ms'] = simulate("round robin", time_quantum=1)
    results['RR-4ms'] = simulate("round robin", time_quantum=4)
    results['SPN'] = simulate("spn")
    results['SRT'] = simulate("srt")
    results['HRRN'] = simulate("hrrn")

    display_results(results)

if __name__ == "__main__":
    main()
