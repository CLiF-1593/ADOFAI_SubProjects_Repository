import video
import painter

deco = {
    "floor": 0,
    "eventType": "AddObject",
    "objectType": "Floor",
    "planetColorType": "DefaultRed",
    "planetColor": "ff0000",
    "planetTailColor": "ff0000",
    "trackType": "Normal",
    "trackAngle": 180,
    "trackColorType": "Single",
    "trackColor": "debb7b00",
    "secondaryTrackColor": "ffffff",
    "trackColorAnimDuration": 2,
    "trackOpacity": 100,
    "trackStyle": "Standard",
    "trackIcon": "None",
    "trackIconAngle": 0,
    "trackRedSwirl": False,
    "trackGraySetSpeedIcon": False,
    "trackSetSpeedIconBpm": 100,
    "trackGlowEnabled": False,
    "trackGlowColor": "ffffff",
    "position": [0, 0],
    "relativeTo": "Tile",
    "pivotOffset": [0, 0],
    "rotation": 0,
    "lockRotation": False,
    "scale": [100, 100],
    "lockScale": False,
    "depth": 0,
    "parallax": [0, 0],
    "parallaxOffset": [0, 0],
    "tag": "",
}

deco_attribute = {
    "floor": 1,
    "eventType": "SetObject",
    "duration": 1,
    "tag": "sampleTag",
    "trackAngle": 180,
    "trackColor": "debb7b",
    "trackOpacity": 100,
    "angleOffset": 0,
    "ease": "Linear",
    "eventTag": "VIDEO",
}

def set_deco_attribute(offset, data, duration, tag, tile_num = 0):
    r = data["color"][0]
    g = data["color"][1]
    b = data["color"][2]
    a = data["color"][3]
    clr = hex(r)[2:].zfill(2) + hex(g)[2:].zfill(2) + hex(b)[2:].zfill(2) + hex(a)[2:].zfill(2)
    deco_attribute["trackColor"] = clr
    deco_attribute["duration"] = 0
    deco_attribute["angleOffset"] = offset
    deco_attribute["tag"] = f"deco{tag}"
    deco_attribute["floor"] = tile_num
    deco_attribute["duration"] = duration
    return deco_attribute

deco_move = {
    "floor": 1,
    "eventType": "MoveDecorations",
    "duration": 1,
    "tag": "sampleTag",
    "visible": True,
    "positionOffset": [0, 0],
    "rotationOffset": 0,
    "scale": [0, 0],
    "parallaxOffset": [0, 0],
    "angleOffset": 0,
    "ease": "OutExpo",
    "eventTag": "VIDEO",
}

def set_deco_move(offset, data, duration, tag, x_offset = 0, y_offset = 0, tile_num = 0):
    x = (data["data"][0] - video.WIDTH / 2) / 100
    y = -(data["data"][1] - video.HEIGHT / 2) / 100
    w = (data["data"][2] - data["data"][0]) * 1.0
    h = (data["data"][3] - data["data"][1]) * 1.75
    x += w / 2 / 100
    y -= h / 2 / 100 / 1.75

    x *= 2
    y *= 2
    w *= 2
    h *= 2

    x += x_offset
    y += y_offset

    if data["type"] == 1:
        rot = 0
    else:
        rot = data["data"][4]
    deco_move["positionOffset"] = [x, y]
    deco_move["rotationOffset"] = -rot
    deco_move["scale"] = [w, h]
    deco_move["angleOffset"] = offset
    deco_move["duration"] = 0
    deco_move["tag"] = f"deco{tag}"
    deco_move["floor"] = tile_num
    deco_move["duration"] = duration
    return deco_move

loop = {
    "floor": 1,
    "eventType": "RepeatEvents",
    "repeatType": "Beat",
    "repetitions": 20,
    "floorCount": 1,
    "interval": 1,
    "executeOnCurrentFloor": False,
    "tag": "VIDEO",
}

tile_move = { "floor": 1, "eventType": "MoveTrack", "startTile": [0, "ThisTile"], "endTile": [0, "ThisTile"], "gapLength": 0, "duration": 1, "positionOffset": [0, 0], "rotationOffset": 0, "angleOffset": 0, "ease": "Linear", "maxVfxOnly": False, "eventTag": "", }

def set_tile_move_to_end(floor, x, y):
    tile_move["floor"] = floor
    tile_move["positionOffset"] = [x - floor, y]
    tile_move["startTile"] = [0, "ThisTile"]
    tile_move["endTile"] = [0, "End"]
    return tile_move