# Map File Grammar

## Comments

Line comments start with `#` and continue to the end of the line.

## Player Start Position [MANDATORY: 1]
The player start position is defined by a line in the format:

```
P (px,py) (lx,ly)
```

Where:
- `P` indicates the player start position.
- `(px,py)` is the player's starting coordinates.
- `(lx,ly)` is the player's initial look direction vector.

## Material Definitions [MANDATORY: 1+]
Materials are defined using the following format:

```
M <Type> "<Name>" <Properties>...
```

Where:
- `M` indicates a material definition.
- `<Type>` is the type of material. Supported types are `Color` and `Texture`.
- `<Name>` is the name of the material, enclosed in double quotes.
- `<Properties>` are the properties of the material, which depend on the type. For `Color`, the properties are the RGB values in the format `(R,G,B)`. For `Texture`, the properties are the file path to the texture image, enclosed in double quotes.

## Sector Definitions [MANDATORY: 1+]
Sectors are defined using the following format:

```
S <SectorID> <FloorHeight> <CeilingHeight> "<FloorMaterial>" "<CeilingMaterial>"
```

Where:
- `S` indicates a sector definition.
- `<SectorID>` is the unique identifier for the sector. 0 is the initial sector where the player starts.
- `<FloorHeight>` is the height of the floor within the sector.
- `<CeilingHeight>` is the height of the ceiling within the sector.
- `<FloorMaterial>` is the name of the material applied to the floor, which must match a previously defined material.
- `<CeilingMaterial>` is the name of the material applied to the ceiling, which must match a previously defined material.

## Wall Definitions [OPTIONAL]
Walls are defined using the following format:

```
W (x1,y1) (x2,y2) "<MaterialName>" <Front> <Back> "<UpperMaterial>" "<LowerMaterial>"
```

Where:
- `W` indicates a wall definition.
- `(x1,y1)` and `(x2,y2)` are the coordinates of the two endpoints of the wall.
- `<MaterialName>` is the name of the material applied to the wall, which must match a previously defined material. If the wall is two-sided, this should be set to `""` (empty string) since the upper and lower materials will be used instead.
- `<Front>` is the sector ID on the front side of the wall.
- `<Back>` is the sector ID on the back side of the wall. If the wall is one-sided, this should be set to `-1`. Otherwise, it should reference a valid sector ID. Walls that contain a back sector are portals that allow the player to see and move between sectors.
- `<UpperMaterial>` is the name of the material applied to the upper part of the wall, which must match a previously defined material.
If the wall is one-sided, this should be set to `""` (empty string). Otherwise, it should reference a valid material name.
- `<LowerMaterial>` is the name of the material applied to the lower part of the wall, which must match a previously defined material. If the wall is one-sided, this should be set to `""` (empty string). Otherwise, it should reference a valid material name.