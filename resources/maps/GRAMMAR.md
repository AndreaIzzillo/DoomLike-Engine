# Map File Grammar

## Comments

Line comments start with `#` and continue to the end of the line.

## Player Start Position
The player start position is defined by a line in the format:

```
P (px,py) (lx,ly)
```

Where:
- `P` indicates the player start position.
- `(px,py)` is the player's starting coordinates.
- `(lx,ly)` is the player's initial look direction vector.

## Material Definitions
Materials are defined using the following format:

```
M <Type> "<Name>" <Properties>...
```

Where:
- `M` indicates a material definition.
- `<Type>` is the type of material. Supported types are `Color` and `Texture`.
- `"<Name>"` is the name of the material, enclosed in double quotes.
- `<Properties>` are the properties of the material, which depend on the type. For `Color`, the properties are the RGB values in the format `(R,G,B)`. For `Texture`, the properties are the file path to the texture image, enclosed in double quotes.

## Wall Definitions
Walls are defined using the following format:

```
W (x1,y1) (x2,y2) <MaterialName>
```

Where:
- `W` indicates a wall definition.
- `(x1,y1)` and `(x2,y2)` are the coordinates of the endpoints of the wall.
- `<MaterialName>` is the name of the material used for the wall, which must match a previously defined material.