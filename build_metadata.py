import sys

try:
    import artifactory_utils
except:
    pass
else:
    dependencies = [
        artifactory_utils.ArtifactSelector(
            project="Toolchain-Release",
            revision="ReleaseMorepork1.12",
            version='^4.6',
            stable_required=True),
        artifactory_utils.ArtifactSelector(
            project="json-cpp",
            revision="ReleaseMorepork2.2",
            stable_required=True)
    ]
    # TODO(chris): Use platform for morepork instead of this hack
    if sys.platform.startswith('linux') and sys.maxsize > 2**32:
        dependencies.extend([
            artifactory_utils.ArtifactSelector(
                project="morepork-json-cpp",
                revision="ReleaseMorepork2.2",
                stable_required=True),
        ])
