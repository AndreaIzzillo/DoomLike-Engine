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
- `<Properties>` are the properties of the material, which depend on the type. For `Color`, the
  properties are the RGB values in the format `(R,G,B)`. For `Texture`, the properties are the file
  path to the texture image, enclosed in double quotes.

To make an `Animated` material, properties must be one image frame followed by the frame duration
in milliseconds. To add more frames, rewrite the line with the same material name and add more
frames.

## Sector Definitions [MANDATORY: 1+]

Sectors are defined using the following format:

```
S <SectorID> <FloorHeight> <CeilingHeight> "<FloorMaterial>" "<CeilingMaterial>" <FloorMatScaleX> <FloorMatScaleY> <FloorMatOffsetX> <FloorMatOffsetY> <CeilingMatScaleX> <CeilingMatScaleY> <CeilingMatOffsetX> <CeilingMatOffsetY>
```

Where:

- `S` indicates a sector definition.
- `<SectorID>` is the unique identifier for the sector. 0 is the initial sector where the player starts.
- `<FloorHeight>` is the height of the floor within the sector.
- `<CeilingHeight>` is the height of the ceiling within the sector.
- `<FloorMaterial>` is the name of the material applied to the floor, which must match a previously defined material.
- `<CeilingMaterial>` is the name of the material applied to the ceiling, which must match a previously defined material.
- Rest of the parameters are the texture scaling and offset parameters for the floor and ceiling materials respectively.

## Wall Definitions [OPTIONAL]

Walls are defined using the following format:

```
W (x1,y1) (x2,y2) "<MaterialName>" <Front> <Back> "<UpperMaterial>" "<LowerMaterial>" <MatScaleX> <MatScaleY> <MatOffsetX> <MatOffsetY> <UpperMatScaleX> <UpperMatScaleY> <UpperMatOffsetX> <UpperMatOffsetY> <LowerMatScaleX> <LowerMatScaleY> <LowerMatOffsetX> <LowerMatOffsetY>
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
- Rest of the parameters are the texture scaling and offset parameters for the main, upper and lower materials respectively.

## Sprite Definitions [OPTIONAL]

Sprites are defined using the following format:

```
T (x,y) "<MaterialName>" <HeightScale> <WidthScale> <VerticalOffset> <SectorID>
```

Where:

- `T` indicates a sprite definition.
- `(x,y)` is the position of the sprite in world coordinates.
- `<MaterialName>` is the name of the material applied to the sprite, which must match a
  previously defined material. For textures, this should be the name of a material of type
  `Texture`. For colored sprites, this should be the name of a material of type `Color`.
- `<HeightScale>` is a multiplier applied to the sprite height.
- `<WidthScale>` is a multiplier applied to the sprite width.
- `<VerticalOffset>` is the vertical offset of the sprite relative to the floor of the sector.
- `<SectorID>` is the ID of the sector the sprite belongs to. This must reference an existing
  sector.

## Light Definitions [OPTIONAL]

Lights are defined using the following format:

```
L (x,y) <Radius> <Intensity> <Color>
```

Where:

- `L` indicates a light definition.
- `(x,y)` is the position of the light in world coordinates.
- `<Radius>` is the radius of the light's effect.
- `<Intensity>` is the intensity of the light, where 1.0 represents full brightness and values less than 1.0 represent dimmer light.
- `<Color>` is the color of the light, represented as an RGB triplet in the format `(R,G,B)`.
