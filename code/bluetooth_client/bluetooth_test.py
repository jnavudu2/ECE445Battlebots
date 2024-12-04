import time
import simplepyble
import logging
from collections import defaultdict
import csv

# Device constants
DEVICE_NAME = "ESP32-BLE"
SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s")

def select_adapter(adapters):
    if len(adapters) == 0:
        logging.error("No adapters found")
        return None
    elif len(adapters) == 1:
        logging.info(f"Found 1 adapter: {adapters[0].identifier()} [{adapters[0].address()}]")
        return adapters[0]
    else:
        logging.info("Please select an adapter:")
        for i, adapter in enumerate(adapters):
            logging.info(f"{i}: {adapter.identifier()} [{adapter.address()}]")
        choice = int(input("Enter choice: "))
        return adapters[choice]

def find_device(adapter):
    # adapter.set_callback_on_scan_start(lambda: logging.info("Scan started."))
    # adapter.set_callback_on_scan_stop(lambda: logging.info("Scan complete."))
    # adapter.set_callback_on_scan_found(lambda peripheral: logging.info(f"Found {peripheral.identifier()} [{peripheral.address()}]"))
    adapter.scan_for(2000)
    peripherals = adapter.scan_get_results()
    for i, peripheral in enumerate(peripherals):
        if peripheral.identifier() == DEVICE_NAME:
            # logging.info(f"Found {DEVICE_NAME} at address [{peripheral.address()}]")
            return peripherals[i]
    logging.error(f"Could not find device with name '{DEVICE_NAME}'")
    return None

def run_range_test(peripheral: simplepyble.Peripheral):
    location_delay = defaultdict(list)

    while True:
        distance = input("Enter distance from receiver ('q' to quit): ")
        if distance == "q":
            break

        for i in range(40):
            message = i.to_bytes(1, "little")
            start = time.perf_counter()
            peripheral.write_request(SERVICE_UUID, CHARACTERISTIC_UUID, message)
            reply = peripheral.read(SERVICE_UUID, CHARACTERISTIC_UUID)
            end = time.perf_counter()
            delay = end - start
            if message == reply:
                location_delay[distance].append(delay)
                logging.info(f"Trial {i}: {delay:.5f} seconds")
            else:
                logging.error(f"Received incorrect reply: {reply} for message: {message}")

    print("Results for 2-way delay:")
    for distance, delays in location_delay.items():
        logging.info(f"Distance: {distance}")
        logging.info(f"Average delay: {sum(delays) / len(delays):.2f} seconds")
        logging.info(f"Minimum delay: {min(delays):.2f} seconds")
        logging.info(f"Maximum delay: {max(delays):.2f} seconds")
    logging.info("Disconnecting...")
    peripheral.disconnect()

    field_names = list(location_delay.keys())

    try:
        with open('bluetooth_delay.csv', 'w', newline='') as csvfile:
            writer = csv.DictWriter(csvfile, fieldnames=field_names)
            
            writer.writeheader()
            
            for i in range(len(location_delay[field_names[0]])):
                writer.writerow({field: location_delay[field][i] for field in field_names})
    except:
        pass

def run_connection_test(adapter, peripheral):
    peripheral.disconnect()

    start_times = []

    trials = 50

    input("Press Enter to run connection test...")
    for i in range(trials):
        start = time.perf_counter()
        peripheral = find_device(adapter)
        peripheral.connect()
        end = time.perf_counter()
        delay = end - start

        start_times.append(delay)
        logging.info(f"Trial {i}: {delay:.5f} seconds")
        peripheral.disconnect()

    logging.info("Results for connection time:")
    logging.info(f"Average delay: {sum(start_times) / len(start_times):.2f} seconds")
    logging.info(f"Minimum delay: {min(start_times):.2f} seconds")
    logging.info(f"Maximum delay: {max(start_times):.2f} seconds")

    try:
        with open('bluetooth_connection.csv', 'w', newline='') as csvfile:
            writer = csv.DictWriter(csvfile, fieldnames=[i for i in range(trials)])
            writer.writeheader()
            writer.writerow({i: start_times[i] for i in range(trials)})
    except:
        pass



def main():
    start = time.perf_counter()
    adapters = simplepyble.Adapter.get_adapters()
    adapter = select_adapter(adapters)
    if not adapter:
        return

    logging.info(f"Selected adapter: {adapter.identifier()} [{adapter.address()}]")
    peripheral = find_device(adapter)
    if not peripheral:
        return

    logging.info(f"Connecting to: {peripheral.identifier()} [{peripheral.address()}]")
    peripheral.connect()
    end = time.perf_counter()
    logging.info("Successfully connected")
    connection_time = end - start
    logging.info(f"Time taken: {connection_time:.2f} seconds")

    # run_range_test(peripheral)
    run_connection_test(adapter, peripheral)
    

if __name__ == "__main__":
    main()
