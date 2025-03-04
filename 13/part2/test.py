
def main():
    # Input file path
    input_file = "C:/Users/Dwight/Desktop/c_dev/AoC/13/part2/build/input.txt"

    # Read the input
    machines = []
    machine = []
    with open(input_file, "r") as f:
        for line in f:
            line = line.strip()
            if not line:
                if machine:
                    machines.append(machine)
                machine = []
                continue
            # Extract numbers from the line
            first = line.find('X') + 2
            comma = line.find(',')
            num1 = int(line[first:comma])
            num2 = int(line[comma + 4:])
            machine.append((num1, num2))

    if machine:
        machines.append(machine)

    total_cost = 0

    for A, B, point in machines:
        Ax, Ay = A  # Coordinates for button A
        Bx, By = B  # Coordinates for button B
        x, y = point  # Current point coordinates

        x_new = x + 10000000000000
        y_new = y + 10000000000000

        # Calculate determinant
        det = Ax * By - Ay * Bx
        if det == 0:
            continue

        # Cramer's rule to calculate x and y
        detx = x_new * By - y_new * Bx
        cramer_x = -1
        if detx % det == 0:  # Check for integer solution
            cramer_x = detx // det

        dety = Ax * y_new - Ay * x_new
        cramer_y = -1
        if dety % det == 0:  # Check for integer solution
            cramer_y = dety // det

        # If both solutions are valid
        if cramer_x >= 0 and cramer_y >= 0:
            cost = 3 * cramer_x + cramer_y
            total_cost += cost

    print(total_cost)


if __name__ == "__main__":
    main()
