# Visualize Langton's ant on a grid

from pathlib import Path
import argparse
from PIL import Image
import numpy as np


def simulate_langtons_ant(width, height, steps, start=None):
	grid = np.zeros((height, width), dtype=np.uint8)
	if start is None:
		x = width // 2
		y = height // 2
	else:
		x, y = start

	# directions: 0=up,1=right,2=down,3=left
	d = 0

	for _ in range(steps):
		color = grid[y, x]
		# on white (0): turn right, flip to black (1)
		if color == 0:
			d = (d + 1) % 4
			grid[y, x] = 1
		else:
			# on black (1): turn left, flip to white (0)
			d = (d - 1) % 4
			grid[y, x] = 0

		# move forward
		if d == 0:
			y -= 1
		elif d == 1:
			x += 1
		elif d == 2:
			y += 1
		else:
			x -= 1

		# wrap around the edges
		x %= width
		y %= height

	return grid, (x, y), d


def grid_to_image(grid, ant_pos=None, ant_color=(255, 0, 0)):
	h, w = grid.shape
	img = np.zeros((h, w, 3), dtype=np.uint8)
	# white background for 0, black for 1
	img[grid == 0] = (255, 255, 255)
	img[grid == 1] = (0, 0, 0)
	if ant_pos is not None:
		x, y = ant_pos
		if 0 <= y < h and 0 <= x < w:
			img[y, x] = ant_color
	return Image.fromarray(img)


def main():
	p = argparse.ArgumentParser(description="Langton's ant visualization")
	p.add_argument('--width', type=int, default=201)
	p.add_argument('--height', type=int, default=201)
	p.add_argument('--steps', type=int, default=11000)
	p.add_argument('--output', type=str, default='langtons_ant.png')
	p.add_argument('--start-x', type=int, default=None)
	p.add_argument('--start-y', type=int, default=None)
	args = p.parse_args()

	width = args.width
	height = args.height
	steps = args.steps

	if args.start_x is None or args.start_y is None:
		start = None
	else:
		start = (args.start_x % width, args.start_y % height)

	grid, ant_pos, _ = simulate_langtons_ant(width, height, steps, start=start)
	print(f"Number of black squares: {np.sum(grid)}")
	
	img = grid_to_image(grid, ant_pos=ant_pos)

	out_path = Path(args.output)
	if not out_path.parent.exists():
		out_path.parent.mkdir(parents=True, exist_ok=True)
	img.save(out_path)
	print(f"Saved visualization to {out_path}")


if __name__ == '__main__':
	main()

