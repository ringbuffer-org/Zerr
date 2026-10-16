{
    "patcher": {
        "fileversion": 1,
        "appversion": {
            "major": 9,
            "minor": 2,
            "revision": 0,
            "architecture": "x64",
            "modernui": 1
        },
        "classnamespace": "box",
        "rect": [ 1785.0, -274.0, 1220.0, 1222.0 ],
        "boxes": [
            {
                "box": {
                    "id": "obj-48",
                    "lastchannelcount": 21,
                    "maxclass": "mc.live.gain~",
                    "numinlets": 1,
                    "numoutlets": 4,
                    "outlettype": [ "multichannelsignal", "", "float", "list" ],
                    "parameter_enable": 1,
                    "patching_rect": [ 55.0, 596.4467005076142, 231.0, 108.0 ],
                    "saved_attribute_attributes": {
                        "valueof": {
                            "parameter_longname": "mc.live.gain~",
                            "parameter_mmax": 6.0,
                            "parameter_mmin": -70.0,
                            "parameter_modmode": 3,
                            "parameter_shortname": "mc.live.gain~",
                            "parameter_type": 0,
                            "parameter_unitstyle": 4
                        }
                    },
                    "varname": "mc.live.gain~"
                }
            },
            {
                "box": {
                    "id": "obj-42",
                    "maxclass": "meter~",
                    "numinlets": 1,
                    "numleds": 20,
                    "numoutlets": 1,
                    "outlettype": [ "float" ],
                    "patching_rect": [ 438.99999999999994, 573.0964467005076, 144.67005076142132, 187.30964467005077 ]
                }
            },
            {
                "box": {
                    "id": "obj-43",
                    "justification": 1,
                    "maxclass": "live.line",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 55.0, 197.8, 891.2000015377998, 13.432835820895521 ]
                }
            },
            {
                "box": {
                    "autofit": 1,
                    "forceaspect": 1,
                    "id": "obj-53",
                    "maxclass": "fpic",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "jit_matrix" ],
                    "patching_rect": [ 843.0, 47.8, 103.20000153779984, 103.20000153779984 ],
                    "pic": "zerr-logo-white-transparent.png"
                }
            },
            {
                "box": {
                    "fontsize": 64.0,
                    "id": "obj-44",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 55.0, 47.8, 862.0, 78.0 ],
                    "text": "Zerr cross jump"
                }
            },
            {
                "box": {
                    "fontsize": 12.0,
                    "id": "obj-33",
                    "linecount": 3,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 55.0, 127.8, 808.5, 47.0 ],
                    "presentation_linecount": 3,
                    "text": "Every zero crossing of the source makes the sound jump to a loudspeaker connected to the current one (trigger mode). The layout is the 21-speaker ambisonic dome and the connections are hand-made, so some speakers are far more likely than others. The source is filtered noise with an LFO on the cutoff."
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-3",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 55.0, 176.8, 760.0, 19.0 ],
                    "text": "https://en.wikipedia.org/wiki/Zero_crossing",
                    "textcolor": [ 0.647, 0.647, 0.647, 1.0 ]
                }
            },
            {
                "box": {
                    "fontface": 1,
                    "id": "obj-5",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 54.31472081218274, 219.28934010152284, 260.0, 20.0 ],
                    "text": "source: noise through a swept low-pass"
                }
            },
            {
                "box": {
                    "format": 6,
                    "id": "obj-6",
                    "maxclass": "flonum",
                    "maximum": 60.0,
                    "minimum": 0.0,
                    "numinlets": 1,
                    "numoutlets": 2,
                    "outlettype": [ "", "bang" ],
                    "parameter_enable": 0,
                    "patching_rect": [ 195.4314720812183, 248.74619289340103, 50.0, 22.0 ]
                }
            },
            {
                "box": {
                    "id": "obj-7",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 247.4314720812183, 249.74619289340103, 120.0, 20.0 ],
                    "text": "LFO (Hz)"
                }
            },
            {
                "box": {
                    "format": 6,
                    "id": "obj-8",
                    "maxclass": "flonum",
                    "maximum": 5000.0,
                    "minimum": 100.0,
                    "numinlets": 1,
                    "numoutlets": 2,
                    "outlettype": [ "", "bang" ],
                    "parameter_enable": 0,
                    "patching_rect": [ 195.4314720812183, 279.71065989847716, 50.0, 22.0 ]
                }
            },
            {
                "box": {
                    "id": "obj-9",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 247.43147208121826, 280.71065989847716, 120.0, 20.0 ],
                    "text": "cutoff centre (Hz)"
                }
            },
            {
                "box": {
                    "format": 6,
                    "id": "obj-10",
                    "maxclass": "flonum",
                    "maximum": 1.0,
                    "minimum": 0.0,
                    "numinlets": 1,
                    "numoutlets": 2,
                    "outlettype": [ "", "bang" ],
                    "parameter_enable": 0,
                    "patching_rect": [ 195.4314720812183, 312.1827411167513, 50.0, 22.0 ]
                }
            },
            {
                "box": {
                    "id": "obj-11",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 247.71573604060916, 313.1979695431472, 120.0, 20.0 ],
                    "text": "resonance 0..1"
                }
            },
            {
                "box": {
                    "id": "obj-12",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 54.31472081218274, 243.19289340101523, 60.0, 22.0 ],
                    "text": "noise~"
                }
            },
            {
                "box": {
                    "id": "obj-13",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 99.81472081218274, 290.8629441624366, 70.0, 22.0 ],
                    "text": "cycle~ 2"
                }
            },
            {
                "box": {
                    "id": "obj-14",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 99.81472081218274, 319.49238578680206, 60.0, 22.0 ],
                    "text": "*~ 400."
                }
            },
            {
                "box": {
                    "id": "obj-15",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 99.81472081218274, 352.28426395939084, 60.0, 22.0 ],
                    "text": "+~ 800."
                }
            },
            {
                "box": {
                    "id": "obj-16",
                    "maxclass": "newobj",
                    "numinlets": 3,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 54.31472081218274, 396.79441624365484, 110.0, 22.0 ],
                    "text": "lores~ 800 0.7"
                }
            },
            {
                "box": {
                    "id": "obj-17",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 54.31472081218275, 432.994923857868, 60.0, 22.0 ],
                    "text": "*~ 0.3"
                }
            },
            {
                "box": {
                    "fontface": 1,
                    "id": "obj-18",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 435.02538071065993, 219.28934010152284, 300.0, 20.0 ],
                    "text": "feature -> pulses -> trigger (inlet 1)"
                }
            },
            {
                "box": {
                    "id": "obj-19",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 435.02538071065993, 244.16243654822335, 130.0, 22.0 ],
                    "text": "zerr.features~ zc"
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-20",
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 574.1116751269036, 246.19289340101523, 303.0, 19.0 ],
                    "text": "zc outputs a pulse at every zero crossing; no scaling needed"
                }
            },
            {
                "box": {
                    "id": "obj-21",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "bang" ],
                    "patching_rect": [ 469.03553299492387, 279.7106598984771, 60.0, 22.0 ],
                    "text": "loadbang"
                }
            },
            {
                "box": {
                    "id": "obj-22",
                    "linecount": 4,
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 469.03553299492387, 324.8730964467005, 444.0, 62.0 ],
                    "text": "topo set 1 21, topo set 2 10, topo set 3 21, topo set 4 12, topo set 5 21, topo set 6 14, topo set 7 21, topo set 8 16, topo set 9 2, topo set 10 1, topo set 11 4, topo set 12 3, topo set 13 6, topo set 14 5, topo set 15 8, topo set 16 7, topo set 17 9 10, topo set 18 11 12, topo set 19 13 14, topo set 20 15 16, topo set 21 20 19 18 17"
                }
            },
            {
                "box": {
                    "id": "obj-23",
                    "maxclass": "message",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "" ],
                    "patching_rect": [ 506.5989847715736, 396.7944162436548, 108.0, 22.0 ],
                    "text": "curr 13, interval 50"
                }
            },
            {
                "box": {
                    "fontsize": 11.0,
                    "id": "obj-24",
                    "linecount": 2,
                    "maxclass": "comment",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 563.4999999999999, 290.8629441624366, 349.535532994924, 31.0 ],
                    "text": "hand-made topology: most speakers lead to 21 at the top, which fans out again; interval 50 caps the jumps at 20 per second"
                }
            },
            {
                "box": {
                    "id": "obj-25",
                    "maxclass": "newobj",
                    "numinlets": 3,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 435.02538071065993, 432.99492385786795, 230.0, 22.0 ],
                    "text": "zerr.envelopes~ trigger ambisonic_21"
                }
            },
            {
                "box": {
                    "id": "obj-26",
                    "maxclass": "newobj",
                    "numinlets": 2,
                    "numoutlets": 1,
                    "outlettype": [ "multichannelsignal" ],
                    "patching_rect": [ 55.0, 512.1827411167512, 110.0, 22.0 ],
                    "text": "zerr.disperser~"
                }
            },
            {
                "box": {
                    "id": "obj-30",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 0,
                    "patching_rect": [ 55.0, 738.4060913705586, 80.0, 22.0 ],
                    "text": "mc.dac~"
                }
            },
            {
                "box": {
                    "id": "obj-31",
                    "maxclass": "newobj",
                    "numinlets": 1,
                    "numoutlets": 1,
                    "outlettype": [ "signal" ],
                    "patching_rect": [ 646.0253807106599, 396.7944162436548, 50.0, 22.0 ],
                    "text": "sig~ 1"
                }
            }
        ],
        "lines": [
            {
                "patchline": {
                    "destination": [ "obj-16", 2 ],
                    "midpoints": [ 204.9314720812183, 383.18432741116743, 154.81472081218274, 383.18432741116743 ],
                    "source": [ "obj-10", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-16", 0 ],
                    "source": [ "obj-12", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-14", 0 ],
                    "source": [ "obj-13", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-15", 0 ],
                    "source": [ "obj-14", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-16", 1 ],
                    "source": [ "obj-15", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-17", 0 ],
                    "source": [ "obj-16", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-19", 0 ],
                    "midpoints": [ 63.81472081218275, 464.994923857868, 390.78965736040607, 464.994923857868, 390.78965736040607, 234.16243654822335, 444.52538071065993, 234.16243654822335 ],
                    "order": 0,
                    "source": [ "obj-17", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-26", 0 ],
                    "order": 1,
                    "source": [ "obj-17", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-25", 0 ],
                    "source": [ "obj-19", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-22", 0 ],
                    "order": 1,
                    "source": [ "obj-21", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-23", 0 ],
                    "midpoints": [ 478.53553299492387, 314.5916878172589, 456.0, 314.5916878172589, 456.0, 393.0, 516.0989847715737, 393.0 ],
                    "order": 0,
                    "source": [ "obj-21", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-25", 0 ],
                    "midpoints": [ 478.53553299492387, 407.243654822335, 444.52538071065993, 407.243654822335 ],
                    "source": [ "obj-22", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-25", 0 ],
                    "midpoints": [ 516.0989847715737, 425.89467005076136, 444.52538071065993, 425.89467005076136 ],
                    "source": [ "obj-23", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-26", 1 ],
                    "midpoints": [ 444.52538071065993, 488.00071383248735, 155.5, 488.00071383248735 ],
                    "source": [ "obj-25", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-42", 0 ],
                    "midpoints": [ 64.5, 554.1154822335025, 448.49999999999994, 554.1154822335025 ],
                    "order": 0,
                    "source": [ "obj-26", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-48", 0 ],
                    "order": 1,
                    "source": [ "obj-26", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-25", 2 ],
                    "source": [ "obj-31", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-30", 0 ],
                    "source": [ "obj-48", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-13", 0 ],
                    "midpoints": [ 204.9314720812183, 282.216687817259, 109.31472081218274, 282.216687817259 ],
                    "source": [ "obj-6", 0 ]
                }
            },
            {
                "patchline": {
                    "destination": [ "obj-15", 1 ],
                    "midpoints": [ 204.9314720812183, 350.09620875634516, 150.31472081218274, 350.09620875634516 ],
                    "source": [ "obj-8", 0 ]
                }
            }
        ],
        "parameters": {
            "obj-48": [ "mc.live.gain~", "mc.live.gain~", 0 ],
            "parameterbanks": {
                "0": {
                    "index": 0,
                    "name": "",
                    "parameters": [ "-", "-", "-", "-", "-", "-", "-", "-" ],
                    "buttons": [ "-", "-", "-", "-", "-", "-", "-", "-" ]
                }
            },
            "inherited_shortname": 1
        },
        "autosave": 0
    }
}