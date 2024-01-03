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

        print(f"Simulation completed at time {self.current_time} ms")

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
                current_process.move_to_next_burst() # may be bug here
                processor.remove_current_process()
                if current_process.has_next_io_burst():
                    io_device = self.select_io_device_for_process(current_process)
                    io_device.add_process(current_process)
                else:
                    current_process.mark_as_complete(self.current_time)

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
        if self.scheduling_algorithm == "round robin":
            self.schedule_round_robin()
        else:
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



def main():
    my_os = OS(num_processors=4, num_io_devices=2)

    processes = [
        Process(1, [('CPU', 48), ('IO1', 12), ('CPU', 36), ('IO2', 16), ('CPU', 12), ('IO1', 12), ('CPU', 24), ('IO1', 14), ('CPU', 48), ('IO1', 12)], arrival_time=0),
        Process(2, [('CPU', 60), ('IO1', 10), ('CPU', 60), ('IO1', 18), ('CPU', 36), ('IO1', 14), ('CPU', 12), ('IO2', 20), ('CPU', 12), ('IO1', 16), ('CPU', 48), ('IO1', 20), ('CPU', 36), ('IO1', 20)], arrival_time=2),
        Process(3, [('CPU', 2), ('IO2', 10), ('CPU', 8), ('IO2', 10), ('CPU', 2), ('IO1', 12), ('CPU', 6), ('IO2', 10), ('CPU', 6), ('IO1', 18), ('CPU', 10), ('IO2', 20)], arrival_time=4),
        Process(4, [('CPU', 24), ('IO2', 20), ('CPU', 48), ('IO2', 14), ('CPU', 36), ('IO1', 10), ('CPU', 24), ('IO1', 18)], arrival_time=6),
        Process(5, [('CPU', 48), ('IO2', 20), ('CPU', 24), ('IO1', 12), ('CPU', 36), ('IO1', 20), ('CPU', 24), ('IO1', 14), ('CPU', 36), ('IO1', 20)], arrival_time=8),
        Process(6, [('CPU', 2), ('IO1', 20), ('CPU', 4), ('IO1', 20), ('CPU', 2), ('IO2', 12), ('CPU', 10), ('IO2', 20), ('CPU', 8), ('IO1', 14)], arrival_time=10)
    ]

    for process in processes:
        my_os.add_process(process)

    my_os.run()

    print("Process Statistics with FCFS Scheduling")
    print(f"{'PID':<12}{'Arrival':<12}{'Service':<12}{'Start':<12}{'Finish':<12}{'Turnaround ':<12}{'Tr/Ts':<12}")
    for process in my_os.all_processes:
        turnaround_time = process.waiting_time + process.service_time
        tr_ts_ratio = turnaround_time / process.service_time if process.service_time > 0 else 0
        print(f"{process.pid:<12}{process.arrival_time:<12}{process.service_time:<12}"
              f"{process.start_time:<12}{process.completion_time:<12}{turnaround_time:<12}{tr_ts_ratio:<12.2f}")



if __name__ == "__main__":
    main()
