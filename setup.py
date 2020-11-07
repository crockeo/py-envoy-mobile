from setuptools import find_packages
from setuptools import setup
from Cython.Build import cythonize

setup(
    name="py-envoy-mobile",
    version="0.0.1",
    description="Python platform implementation of envoy-mobile",
    keywords="",
    author="Cerek Hillen",
    author_email="chillen@lyft.com",
    url="https://github.com/crockeo/py-envoy-mobile",
    packages=find_packages(),
    ext_modules=cythonize(
        [
            # TODO
        ],
    ),
    include_package_data=True,
)
