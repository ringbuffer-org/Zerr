import math
import csv
import argparse

class CartesianPoint:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

class SphericalPoint:
    def __init__(self, azi, elev, dist):
        self.azi = azi
        self.elev = elev
        self.dist = dist

def spherical_to_cartesian(spherical):
    x = spherical.dist * math.cos(math.radians(spherical.elev)) * math.cos(math.radians(spherical.azi))
    y = spherical.dist * math.cos(math.radians(spherical.elev)) * math.sin(math.radians(spherical.azi))
    z = spherical.dist * math.sin(math.radians(spherical.elev))

    return CartesianPoint(x, y, z)


def main(input_file, output_file):
    spherical_data = []
    cartesian_data = []

    # Read input CSV file
    with open(input_file, 'r') as file:
        reader = csv.DictReader(file)
        for row in reader:
            spherical_data.append(row)
    
    for spherical_row in spherical_data:
        idx = int(spherical_row['index'])
        azi = float(spherical_row['azi'])
        elev = float(spherical_row['elev'])
        dist = float(spherical_row['dist'])

        spherical = SphericalPoint(azi, elev, dist)

        cartesian = spherical_to_cartesian(spherical)


        # Create a new row with translated values
        cartesian_row = {
            'index':str(idx),
            'x':    format(cartesian.x, '.3f'),
            'y':    format(cartesian.y, '.3f'),
            'z':    format(cartesian.z, '.3f')
        }
        cartesian_data.append(cartesian_row)

    # Write translated data to output CSV file
    fieldnames = ['index', 'x', 'y', 'z']
    with open(output_file, 'w', newline='') as file:
        writer = csv.DictWriter(file, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(cartesian_data)

    print("Transform complete. Output file:", output_file)


if __name__ == '__main__':
    # input_file  = 'EN325_Dome_spherical.csv'
    # output_file = 'EN325_Dome_cartesian.csv'

    # Command-line argument parsing
    parser = argparse.ArgumentParser(description='Translate data in a CSV file.')
    parser.add_argument('-i', '--input', type=str, help='input CSV file')
    parser.add_argument('-o', '--output', type=str, help='output CSV file')
    args = parser.parse_args()

    # Call the translate_data function with the provided input and output file names
    main(args.input, args.output)