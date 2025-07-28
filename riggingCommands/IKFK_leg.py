def IKFKleg(name, shape, lock = []):
    ctrl = cmds.curve(shape)
    grp = cmds.group(ctrl, n = name.replace(CONTROL, GROUP))

    for transform in lock:
        cmds.setAttr(f'{ctrl}.{transform}', lock=True, keyable=False, channelBox=False)

    return grp, ctrl
#INFO making a leg woopwoop

