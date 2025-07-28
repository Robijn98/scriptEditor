import maya.cmds as cmds
import sys
from controller import create_temp_ctrl
from controller import add_offset_jnt
from controller import add_offset

class jaw:

    GROUP = 'GRP'
    JOINT = 'JNT'
    GUIDE = 'GUIDE'
    JAW = 'jaw'
    CONTROL = 'CTRL'
    #side constants
    LEFT = 'L'
    RIGHT = 'R'
    CENTER = 'C'

    def __init__(self, side, name, number=5):
        self.side = side
        self.name = name
        self.number = number

    
    def lip_guides(self):
        grp = f'{CENTER}_{JAW}_lipMinor_{GUIDE}_{GROUP}'
        guides = []

        if cmds.objExists(grp):
            for loc in cmds.listRelatives(grp):
                guides.append(loc)

        return guides

    def jaw_guides(self):
        grp = f'{CENTER}_{JAW}_base_{GUIDE}_{GROUP}'
        guides = []

        if cmds.objExists(grp):
            for loc in cmds.listRelatives(grp, c=True):
                guides.append(loc)

        return guides

    def create_hierachy(self):
        main_grp = cmds.createNode('transform', name=f'{CENTER}_{JAW}_rig_{GROUP}')
        lip_grp = cmds.createNode('transform', name=f'{CENTER}_{JAW}_lip_{GROUP}', parent = main_grp)
        main_grp = cmds.createNode('transform', name=f'{CENTER}_{JAW}Base_{GROUP}', parent = main_grp)

        lip_minor_grp = cmds.createNode('transform', name=f'{CENTER}_{JAW}_lip_minor_{GROUP}', parent = lip_grp)
        lip_broad_grp = cmds.createNode('transform', name=f'{CENTER}_{JAW}_lip_broad_{GROUP}', parent = lip_grp)

        cmds.select(cl=True)

    def create_minor_jnts(self):
            minor_joints = []
            guides = self.lip_guides()
    
            for guide in guides:
                mat = cmds.xform(guide, q=True, m=True, ws=True)
                jnt = cmds.joint(name = guide.replace(GUIDE, JOINT))
                cmds.setAttr(f'{jnt}.radius', 0.5)
                cmds.xform(jnt, m=mat, ws=True)
    
                cmds.parent(jnt, f'{CENTER}_{JAW}_lip_minor_{GROUP}')
    
                minor_joints.append(jnt)
    
            return minor_joints

    def create_broad_jnts(self):

        upper_jnt = cmds.joint(name = f'{CENTER}_{JAW}_broadUpper_{JOINT}')
        cmds.select(cl=True)
        lower_jnt = cmds.joint(name = f'{CENTER}_{JAW}_broadLower_{JOINT}')
        cmds.select(cl=True)
        left_jnt = cmds.joint(name = f'{LEFT}_{JAW}_broadCorner_{JOINT}')
        cmds.select(cl=True)
        right_jnt = cmds.joint(name = f'{RIGHT}_{JAW}_broadCorner_{JOINT}')
        cmds.select(cl=True)

        #parent joints under broad group
        cmds.parent([upper_jnt, lower_jnt, left_jnt, right_jnt], f'{CENTER}_{JAW}_lip_broad_{GROUP}')

        #get guides positions
        upper_pos = cmds.xform(self.lip_guides()[0], q=True, m=True, ws=True)
        lower_pos = cmds.xform(self.lip_guides()[1], q=True, m=True, ws=True)
        left_pos = cmds.xform(self.lip_guides()[2], q=True, m=True, ws=True)
        right_pos = cmds.xform(self.lip_guides()[3], q=True, m=True, ws=True)

        #set joint positions
        cmds.xform(upper_jnt, m=upper_pos)
        cmds.xform(lower_jnt, m=lower_pos)
        cmds.xform(left_jnt, m=left_pos)
        cmds.xform(right_jnt, m=right_pos)

        cmds.select(cl=True)

    def create_jaw_base(self):
        jaw_jnt = cmds.joint(name = f'{CENTER}_{JAW}_{JOINT}')
        cmds.select(cl=True)
        jaw_inverse_jnt = cmds.joint(name = f'{CENTER}_inverse_{JAW}_{JOINT}')
        cmds.select(cl=True)

        jaw_mat = cmds.xform(self.jaw_guides()[0], q=True, m=True, ws=True)
        jaw_inverse_mat = cmds.xform(self.jaw_guides()[1], q=True, m=True, ws=True)

        cmds.xform(jaw_jnt, m=jaw_mat)
        cmds.xform(jaw_inverse_jnt, m=jaw_inverse_mat)

        cmds.parent(jaw_jnt, f'{CENTER}_{JAW}Base_{GROUP}')
        cmds.parent(jaw_inverse_jnt, f'{CENTER}_{JAW}Base_{GROUP}')

        cmds.select(cl=True)

        #add offsets
        add_offset(jaw_jnt, suffix = 'OFF')
        add_offset(jaw_inverse_jnt, suffix = 'OFF')

        add_offset(jaw_jnt, suffix = 'AUTO')
        add_offset(jaw_inverse_jnt, suffix = 'AUTO')

        cmds.select(cl=True)

    def constraint_broad_joints(self):

        jaw_jnt = f'{CENTER}_{JAW}_{JOINT}'
        inverse_jaw_jnt = f'{CENTER}_inverse_{JAW}_{JOINT}'

        broad_upper = f'{CENTER}_{JAW}_broadUpper_{JOINT}'
        broad_lower = f'{CENTER}_{JAW}_broadLower_{JOINT}'
        broad_left = f'{LEFT}_{JAW}_broadCorner_{JOINT}'
        broad_right = f'{RIGHT}_{JAW}_broadCorner_{JOINT}'

        #add offsets
        upper_off = add_offset(broad_upper)
        lower_off = add_offset(broad_lower)
        left_off = add_offset(broad_left)
        right_off = add_offset(broad_right)

        #constraining joints
        cmds.parentConstraint(jaw_jnt, lower_off, mo=True)
        cmds.parentConstraint(inverse_jaw_jnt, upper_off, mo=True)
        cmds.parentConstraint(upper_off, lower_off, left_off, mo=True)
        cmds.parentConstraint(upper_off, lower_off,right_off, mo=True)

        cmds.select(cl=True)

    def get_lip_parts(self):

        upper_token = 'jawupper'
        lower_token = 'jawlower
        corner_token = 'jawCorner'

        C_upper = f'{CENTER}_{JAW}_broadUpper_{JOINT}'
        C_lower = f'{CENTER}_{JAW}_broadLower_{JOINT}'
        R_corner = f'{RIGHT}_{JAW}_broadCorner_{JOINT}'
        L_corner = f'{LEFT}_{JAW}_broadCorner_{JOINT}'

        lip_joints = cmds.listRelatives(f'{CENTER}_{JAW}_lip_{GROUP}', allDescendents=True)

        dict_lips = {'C_upper': {}, 'C_lower': {},
                    'L_upper': {}, 'L_lower': {},
                    'R_upper': {}, 'R_lower': {},
                    'L_corner': {}, 'R_corner': {}}

        for joint in lip_joints:
                
                if cmds.objectType(joint) != 'joint':
                    continue
    
                if joint.startswith('C') and upper_token in joint:
                    dict_lips['C_upper'][joint] = [C_upper]
    
                if joint.startswith('C') and lower_token in joint:
                    dict_lips['C_lower'][joint] = [C_lower]
    
                if joint.startswith('L') and upper_token in joint:
                    dict_lips['L_upper'][joint] = [C_upper, L_corner]
    
                if joint.startswith('L') and lower_token in joint:
                    dict_lips['L_lower'][joint] = [C_lower, L_corner]
    
                if joint.startswith('R') and upper_token in joint:
                    dict_lips['R_upper'][joint] = [C_upper, R_corner]
    
                if joint.startswith('R') and lower_token in joint:
                    dict_lips['R_lower'][joint] = [C_lower, R_corner]
    
                if joint.startswith('L') and corner_token in joint:
                    dict_lips['L_corner'][joint] = [L_corner]
    
                if joint.startswith('R') and corner_token in joint:
                    dict_lips['R_corner'][joint] = [R_corner]

        return dict_lips


    def lip_parts(self, part):

        lip_parts = [reversed(sorted(self.get_lip_parts()[f'L_{part}'].keys())), self.get_lip_parts()[f'C_{part}'].keys(),
                    sorted(self.get_lip_parts()[f'R_{part}'].keys())]

        return [joint for joint in lip_parts for joint in joint]


    def create_seals(self, part):

        seal_name = f'{CENTER}_seal_{GROUP}'
        seal_parent = seal_name if cmds.objExists(seal_name) else \
            cmds.createNode('transform', name = seal_name, parent = f'{CENTER}_{JAW}_rig_{GROUP}')

        part_grp = cmds.createNode('transform', name = seal_name.replace('seal', f'seal_{part}'), parent = seal_parent)

        l_corner = f'{LEFT}_{JAW}_broadCorner_{JOINT}'
        r_corner = f'{RIGHT}_{JAW}_broadCorner_{JOINT}'

        value = len(lip_parts(part))

        for index, joint in enumerate(lip_parts(part)):
            node = cmds.createNode('transform', name = joint.replace('JNT', f'{part}_seal'), parent = part_grp)
            mat = cmds.xform(joint, q=True, m=True, ws=True)
            cmds.xform(node, m=mat)

            constraint = cmds.parentConstraint(l_corner, r_corner, node, mo=True)[0]
            cmds.setAttr(f'{constraint}.interpType', 2)

            r_corner_value = float(index)/float(value - 1)
            l_corner_value = 1 - r_corner_value

            l_corner_attr = f'{constraint}.{l_corner}W0'
            r_corner_attr = f'{constraint}.{r_corner}W1'

            cmds.setAttr(l_corner_attr, l_corner_value)
            cmds.setAttr(r_corner_attr, r_corner_value)

        cmds.select(cl=True)


    def create_jaw_attrs(self):

        node = cmds.createNode('transform', name = 'jaw_attributes', parent = f'{CENTER}_{JAW}_rig_{GROUP}')
        cmds.addAttr(node, ln= sorted(self.get_lip_parts()['C_upper'].keys())[0], min=0, max=1, dv=0)
        cmds.setAttr(f"{node}.{sorted(self.get_lip_parts()['C_upper'].keys())[0]}", lock=1)

        for upper in sorted(self.get_lip_parts()['L_upper'].keys()):
            cmds.addAttr(node, ln=upper, min=0, max=1, dv=0)

        cmds.addAttr(node, ln= sorted(self.get_lip_parts()['L_corner'].keys())[0], min=0, max=1, dv=1)
        cmds.setAttr(f"{node}.{sorted(self.get_lip_parts()['L_corner'].keys())[0]}", lock=1)

        for lower in sorted(self.get_lip_parts()['L_lower'].keys())[::-1]:
            cmds.addAttr(node, ln=lower, min=0, max=1, dv=0)

        cmds.addAttr(node, ln= sorted(self.get_lip_parts()['C_lower'].keys())[0], min=0, max=1, dv=0)
        cmds.setAttr(f"{node}.{sorted(self.get_lip_parts()['C_lower'].keys())[0]}", lock=1)

        self.create_offset_follow()
        self.add_seal_attr()


    def create_constraint(self):

        for value in self.get_lip_parts().values():
            for lip_jnt, broad_jnt in value.items():

                seal_token = 'upper_seal' if 'upper' in lip_jnt else 'lower_seal'
                lip_seal = lip_jnt.replace(JOINT, seal_token)

                if cmds.objExists(lip_seal):
                    const = cmds.parentConstraint(broad_jnt, lip_seal, lip_jnt, mo=True)[0]
                    cmds.setAttr(f'{const}.interpType', 2)

                    if len(broad_jnt) == 1:
                        seal_attr = f'{lip_jnt}_parentConstraint1.{lip_seal}W1'
                        rev = cmds.createNode('reverse', name= lip_jnt.replace(JOINT, 'REV'))
                        cmds.connectAttr(seal_attr, f'{rev}.inputX')
                        cmds.connectAttr(f'{rev}.outputX', f'{lip_jnt}_parentConstraint1.{broad_jnt[0]}W0')
                        cmds.setAttr(seal_attr, 0)

                    if len(broad_jnt) == 2:
                        seal_attr = f'{lip_jnt}_parentConstraint1.{lip_seal}W2'
                        cmds.setAttr(seal_attr, 0)

                        seal_rev = cmds.createNode('reverse', name=lip_jnt.replace(JOINT, 'seal_REV'))
                        jaw_attr_rev = cmds.createNode('reverse', name=lip_jnt.replace(JOINT, 'jaw_attr_REV'))
                        seal_mult = cmds.createNode('multiplyDivide', name=lip_jnt.replace(JOINT, 'seal_MULT'))

                        cmds.connectAttr(seal_attr, f'{seal_rev}.inputX')
                        cmds.connectAttr(f'{seal_rev}.outputX', f'{seal_mult}.input2X')
                        cmds.connectAttr(f'{seal_rev}.outputX', f'{seal_mult}.input2Y')

                        cmds.connectAttr(f'jaw_attributes.{lip_jnt.replace(lip_jnt[0], "L")}', f'{seal_mult}.input1Y')

                        cmds.connectAttr(f'jaw_attributes.{lip_jnt.replace(lip_jnt[0], "L")}', f'{jaw_attr_rev}.inputX')

                        cmds.connectAttr(f'{jaw_attr_rev}.outputX', f'{seal_mult}.input1X')

                        cmds.connectAttr(f'{seal_mult}.outputX', f'{lip_jnt}_parentConstraint1.{broad_jnt[0]}W0')

                        cmds.connectAttr(f'{seal_mult}.outputY', f'{lip_jnt}_parentConstraint1.{broad_jnt[1]}W1')

                else:
                    const = cmds.parentConstraint(broad_jnt, lip_jnt, mo=True)[0]
                    cmds.setAttr(f'{const}.interpType', 2)


    def initial_values_jaw(self, degree=1.3):

        jaw_attr = [part for part in self.lip_parts(part) if not part.startswith('C') and not part.startswith('R')]
        value = len(jaw_attr)

        for index, attr_name in enumerate(jaw_attr[::-1]):
            attr = f'jaw_attributes.{attr_name}'

            linear_value = float(index) / float(value - 1)
            div_value = linear_value / degree
            final_value = div_value * linear_value

            cmds.setAttr(attr, final_value)


    def create_offset_follow(self):
            
            jaw_attr = 'jaw_attributes'
    
            jaw_joint = f'{CENTER}_{JAW}_{JOINT}'
            jaw_auto = f'{CENTER}_{JAW}_{JOINT}_AUTO'
    
            #add follow constraints
            cmds.addAttr(jaw_attr, ln = 'follow_ty', min = -10, max=10, dv=0)
            cmds.addAttr(jaw_attr, ln = 'follow_tz', min = -10, max=10, dv=0)
    
            unit = cmds.createNode('unitConversion', name = f'{CENTER}_{JAW}_follow_UNIT')
    
            remap_y = cmds.createNode('remapValue', name = f'{CENTER}_{JAW}_followY_remap')
            cmds.setAttr(f'{remap_y}.inputMax', 1)
    
            remap_z = cmds.createNode('remapValue', name=f'{CENTER}_{JAW}_followY_remap')
            cmds.setAttr(f'{remap_z}.inputMax', 1)
    
            mult_y = cmds.createNode('multDoubleLinear', name = f'{CENTER}_{JAW}_followY_MULT')
            cmds.setAttr(f'{mult_y}.input2', 1)
    
            cmds.connectAttr(f'{jaw_joint}.rx', f'{unit}.input')
            cmds.connectAttr(f'{unit}.output', f'{remap_y}.inputValue')
            cmds.connectAttr(f'{unit}.output', f'{remap_z}.inputValue')
    
            cmds.connectAttr(f'{jaw_attr}.follow_ty', f'{mult_y}.input1')
            cmds.connectAttr(f'{jaw_attr}.follow_tz', f'{remap_z}.outputMax')
            cmds.connectAttr(f'{mult_y}.output', f'{remap_y}.outputMax')
    
            cmds.connectAttr(f'{remap_y}.outValue', f'{jaw_auto}.ty')
            cmds.connectAttr(f'{remap_z}.outValue', f'{jaw_auto}.tz')



    def add_seal_attr(self):

        jaw_attr = 'jaw_attributes'

        cmds.addAttr(jaw_attr, at='double', ln='L_seal', min=0, max=10, dv=0)
        cmds.addAttr(jaw_attr, at='double', ln='R_seal', min=0, max=10, dv=0)

        cmds.addAttr(jaw_attr, at='double', ln='L_seal_delay', min=0.1, max=10, dv=4)
        cmds.addAttr(jaw_attr, at='double', ln='R_seal_delay', min=0.1, max=10, dv=4)


    def connect_seal(self, part):

        seal_token = f'seal_{part}'

        jaw_attrs = 'jaw_attributes'
        lip_jnts = lipParts(part)
        value = len(lip_jnts)
        seal_driver = cmds.createNode('lightInfo', name=f'C_{seal_token}_DRV')

        triggers = {'L': list(), 'R': list()}

        for side in ['L','R']:
            delay_sub_name = f'{side}_{seal_token}_delay_SUB'
            delay_sub = cmds.createNode('plusMinusAverage', name=delay_sub_name)

            cmds.setAttr(f'{delay_sub}.operation', 2)
            cmds.setAttr(f'{delay_sub}.input1D[0]', 10)
            cmds.connectAttr(f'{jaw_attrs}.{side}_seal_delay', f'{delay_sub}.input1D[1]')

            lerp = 1 / float(value - 1)

            delay_div_name = f'{side}_{seal_token}_delay_DIV'
            delay_div = cmds.createNode('multDoubleLinear', name= delay_div_name)for side in ['L','R']:
            delay_sub_name = f'{side}_{seal_token}_delay_SUB'
            delay_sub = cmds.createNode('plusMinusAverage', name=delay_sub_name)
                delay_mult = cmds.createNode('multDoubleLinear', n=delay_mult_name)
                cmds.setAttr(f'{delay_mult}.input1', index)
                cmds.connectAttr(f'{delay_div}.output', f'{delay_mult}.input2')

                mult_triggers.append(delay_mult)

                #create SUB Mode
                delay_sub_name = f'{index_name}_{side}_{seal_token}_delay_SUB'
                delay_sub = cmds.createNode('plusMinusAverage', n=delay_sub_name)
                cmds.connectAttr(f'{delay_mult}.output', f'{delay_sub}.input1D[0]')
                cmds.connectAttr(f'{jaw_attrs}.{side}_seal_delay', f'{delay_sub}.input1D[1]')

                sub_triggers.append(delay_sub)

        #get constraints
        const_targets = []

        for jnt in lip_jnts:
            attrs = cmds.listAttr(f'{jnt}_parentConstraint1', ud=True)
            for attr in attrs:
                if 'seal' in attr:
                    const_targets.append(f'{jnt}_parentConstraint1.{attr}')

        #connect seal triggers to driver node
        for left_index, const_target in enumerate(const_targets):
            right_index = value - left_index - 1
            index_name = f'{seal_token}_{left_index}'

            l_mult_trigger, l_sub_trigger = triggers['L'][0][left_index], triggers['L'][1][left_index]
            r_mult_trigger, r_sub_trigger = triggers['R'][0][right_index], triggers['R'][1][right_index]

            #left
            l_remap_name = f'L_{seal_token}_{index_name}_REMAP'
            l_remap = cmds.createNode('remapValue', name = l_remap_name)
            cmds.setAttr(f'{l_remap}.outputMax', 1)
            cmds.setAttr(f'{l_remap}.value[0].value_Interp', 2)

            cmds.connectAttr(f'{l_mult_trigger}.output', f'{l_remap}.inputMin')
            cmds.connectAttr(f'{l_sub_trigger}.output1D', f'{l_remap}.inputMax')

            #connect left seal attribute to input of remap
            cmds.connectAttr(f'{jaw_attrs}.L_seal', f'{l_remap}.inputValue')

            #right
            #substract 1 minus result from left remap
            r_sub_name = f'R_{seal_token}_offset_{index_name}_SUB'
            r_sub = cmds.createNode('plusMinusAverage', name=r_sub_name)
            cmds.setAttr(f'{r_sub}.input1D[0]', 1)
            cmds.setAttr(f'{r_sub}.operation', 2)

            cmds.connectAttr(f'{l_remap}.outValue', f'{r_sub}.input1D[1]')
            r_remap_name = f'R_{seal_token}_{index_name}_REMAP'
            r_remap = cmds.createNode('remapValue', name = r_remap_name)
            cmds.setAttr(f'{r_remap}.outputMax', 1)
            cmds.setAttr(f'{r_remap}.value[0].value_Interp', 2)

            cmds.connectAttr(f'{r_mult_trigger}.output', f'{r_remap}.inputMin')
            cmds.connectAttr(f'{r_sub_trigger}.output1D', f'{r_remap}.inputMax')

            # connect right seal attribute to input of remap
            cmds.connectAttr(f'{jaw_attrs}.R_seal', f'{r_remap}.inputValue')

            cmds.connectAttr(f'{r_sub}.output1D', f'{r_remap}.outputMax')

            #final addition both sides
            plus_name = f'{index_name}_SUM'
            plus = cmds.createNode('plusMinusAverage', name = plus_name)

            cmds.connectAttr(f'{l_remap}.outValue', f'{plus}.input1D[0]')
            cmds.connectAttr(f'{r_remap}.outValue', f'{plus}.input1D[1]')

            clamp_name = f'{index_name}_CLAMP'
            clamp = cmds.createNode('remapValue', name = clamp_name)
            cmds.connectAttr(f'{plus}.output1D', f'{clamp}.inputValue')

            cmds.addAttr(seal_driver, at='double', ln=index_name, min=0, max=1, dv=0)
            cmds.connectAttr(f'{clamp}.outValue', f'{seal_driver}.{index_name}')

            cmds.connectAttr(f'{seal_driver}.{index_name}', const_target)

    def create_jaw_pins(self):
        
        pin_driver = cmds.createNode('lightInfo', name = f'{CENTER}_pin_DRV')

        for side in ['L', 'R']:

            jaw_attr = 'jaw_attributes'

            cmds.addAttr(jaw_attr, at='bool', ln=f'{side}_auto_corner_pin')
            cmds.addAttr(jaw_attr, at='double', ln=f'{side}_corner_pin', min=-10, max=10, dv=0)
            cmds.addAttr(jaw_attr, at='double', ln=f'{side}_input_ty', min=-10, max=10, dv=0)

            #create clamp and connect imput_ty to it
            clamp = cmds.createNode('clamp', name = f'{side}_corner_pin_auto_CLAMP')
            cmds.setAttr(f'{clamp}.minR', -10)
            cmds.setAttr(f'{clamp}.maxR', 10)

            cmds.connectAttr(f'{jaw_attr}.{side}_input_ty', f'{clamp}.inputR')

            #create condition for the two possible scenarios
            cnd = cmds.createNode('condition', name = f'{side}_corner_pin_auto_CND')
            cmds.setAttr(f'{cnd}.operation', 0)
            cmds.setAttr(f'{cnd}.secondTerm', 1)

            cmds.connectAttr(f'{jaw_attr}.{side}_auto_corner_pin', f'{cnd}.firstTerm')
            cmds.connectAttr(f'{clamp}.outputR', f'{cnd}.colorIfTrueR')
            cmds.connectAttr(f'{jaw_attr}.{side}_corner_pin', f'{cnd}.colorIfFalseR')

            #create addition
            plus = cmds.createNode('plusMinusAverage', name= f'{side}_corner_pin_PLUS')
            cmds.setAttr(f'{plus}.input1D[1]', 10)
            cmds.connectAttr(f'{cnd}.outColorR', f'{plus}.input1D[0]')

            #create division
            div = cmds.createNode('multDoubleLinear', name= f'{side}_corner_pin_DIV')
            cmds.setAttr(f'{div}.input2', 0.05)
            cmds.connectAttr(f'{plus}.output1D', f'{div}.input1')

            #add final output attributes to the driver node
            cmds.addAttr(pin_driver, at='double', ln=f'{side}_pin', min=0, max=1, dv=0)
            cmds.connectAttr(f'{div}.output', f'{pin_driver}.{side}_pin')

            #connect driver
            const_pin_up = f'{side}_jaw_broadCorner_JNT_OFF_parentConstraint1.C_jaw_broadUpper_JNT_OFFW0'
            const_pin_down = f'{side}_jaw_broadCorner_JNT_OFF_parentConstraint1.C_jaw_broadLower_JNT_OFFW1'

            cmds.connectAttr(f'{pin_driver}.{side}_pin', const_pin_up)

            rev = cmds.createNode('reverse', name = f'{side}_corner_pin_REV')
            cmds.connectAttr(f'{pin_driver}.{side}_pin', f'{rev}.inputX')
            cmds.connectAttr(f'{rev}.outputX', const_pin_down)

    def create_jaw_ctrl(self):
        jaw_grp, jaw_ctrl = create_temp_ctrl(f'{CENTER}_{JAW}_{CONTROL}', lock = ['sx', 'sy', 'sz', 'tx', 'ty', 'tz'])

        mat = cmds.xform(f'{CENTER}_{JAW}_{JOINT}', q=True, m=True, ws=True)
        cmds.xform(jaw_grp, m=mat, ws=True)

        cmds.parentConstraint(jaw_ctrl, f'{CENTER}_{JAW}_{JOINT}_OFF', mo=True)

        cmds.addAttr(jaw_ctrl, ln='follow_TY', at='double', min=-10, max=10, dv =0, k=True, h=False)
        cmds.addAttr(jaw_ctrl, ln='follow_TZ', at='double', min=-10, max=10, dv =0, k=True, h=False)

        cmds.addAttr(jaw_ctrl, ln=f'{LEFT}_seal', at='double', min=0, max=10, dv =0, k=True, h=False)
        cmds.addAttr(jaw_ctrl, ln=f'{RIGHT}_seal', at='double', min=0, max=10, dv =0, k=True, h=False)

        cmds.addAttr(jaw_ctrl, ln=f'{LEFT}_seal_delay', at='double', min=0.1, max=10, dv =4, k=True, h=False)
        cmds.addAttr(jaw_ctrl, ln=f'{RIGHT}_seal_delay', at='double', min=0.1, max=10, dv =4, k=True, h=False)

        cmds.addAttr(jaw_ctrl, ln=f'{LEFT}_autocorner_pin', at='enum', en = "off:on", k=True, h=False)
        cmds.addAttr(jaw_ctrl, ln=f'{LEFT}_corner_pin', at='double', min=-10, max=10, dv =0, k=True, h=False)
        cmds.addAttr(jaw_ctrl, ln=f'{LEFT}_input_TY', at='double', min=-10, max=10, dv =0, k=True, h=False)

        cmds.addAttr(jaw_ctrl, ln=f'{RIGHT}_autocorner_pin', at='enum', en = "off:on", k=True, h=False)
        cmds.addAttr(jaw_ctrl, ln=f'{RIGHT}_corner_pin', at='double', min=-10, max=10, dv =0, k=True, h=False)
        cmds.addAttr(jaw_ctrl, ln=f'{RIGHT}_input_TY', at='double', min=-10, max=10, dv =0, k=True, h=False)

        cmds.connectAttr(f'{jaw_ctrl}.follow_TY', 'jaw_attributes.follow_ty')
        cmds.connectAttr(f'{jaw_ctrl}.follow_TZ', 'jaw_attributes.follow_tz')

        cmds.connectAttr(f'{jaw_ctrl}.{LEFT}_seal', 'jaw_attributes.L_seal')
        cmds.connectAttr(f'{jaw_ctrl}.{RIGHT}_seal', 'jaw_attributes.R_seal')
        cmds.connectAttr(f'{jaw_ctrl}.{LEFT}_seal_delay', 'jaw_attributes.L_seal_delay')
        cmds.connectAttr(f'{jaw_ctrl}.{RIGHT}_seal_delay', 'jaw_attributes.R_seal_delay')

        cmds.connectAttr(f'{jaw_ctrl}.{LEFT}_autocorner_pin', 'jaw_attributes.L_auto_corner_pin')
        cmds.connectAttr(f'{jaw_ctrl}.{LEFT}_corner_pin', 'jaw_attributes.L_corner_pin')
        cmds.connectAttr(f'{jaw_ctrl}.{LEFT}_input_TY', 'jaw_attributes.L_input_ty')

        cmds.connectAttr(f'{jaw_ctrl}.{RIGHT}_autocorner_pin', 'jaw_attributes.R_auto_corner_pin')
        cmds.connectAttr(f'{jaw_ctrl}.{RIGHT}_corner_pin', 'jaw_attributes.R_corner_pin')
        cmds.connectAttr(f'{jaw_ctrl}.{RIGHT}_input_TY', 'jaw_attributes.R_input_ty')

    def jaw(self):
        self.create_hierachy()
        self.create_minor_jnts()
        self.create_broad_jnts()
        self.create_jaw_base()
        self.constraint_broad_joints()
        self.create_seals('lower')
        self.create_seals('upper')
        self.create_jaw_attrs()
        self.create_constraint()
        self.initial_values_jaw('upper')
        self.initial_values_jaw('lower')
        self.connect_seal('lower')
        self.connect_seal('upper')
        self.create_jaw_pins()
        self.create_jaw_ctrl()

#CLASS AND FUNCTION
#jaw.jaw()
#INFO im a jaw :)