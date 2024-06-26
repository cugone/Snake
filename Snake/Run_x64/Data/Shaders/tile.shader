<shader name="tile">
    <shaderprogram src="Data/ShaderPrograms/tile_VS.cso" />
    <shaderprogram src="Data/ShaderPrograms/tile_PS.cso" />
    <raster>
        <fill>solid</fill>
        <cull>none</cull>
        <antialiasing>false</antialiasing>
    </raster>
    <sampler>
        <filter min="point" mag="point" mip="point" mode="none" />
    </sampler>
    <blends>
        <blend enable="true">
            <color src="src_alpha" dest="inv_src_alpha" op="add" />
        </blend>
    </blends>
    <depth enable="false" writable="false" />
    <stencil enable="false" readable="false" writable="false" />
</shader>
